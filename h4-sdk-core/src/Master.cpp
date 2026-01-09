//
// Created by srfas on 12/13/2025.
//

#include "Master.h"

#include <cinttypes>
#include <cstring>

// Static member definitions
std::uint8_t Master::IOmap[IOMAP_SIZE] = {};
OSAL_THREAD_HANDLE Master::threadrt, Master::thread1;

int Master::expectedWKC = 0;
int Master::wkc = 0;
int Master::mappingdone = 0;
int Master::dorun = 0;
int Master::inOP = 0;
int Master::dowkccheck = 0;

int64 Master::timeerror = 0;

ecx_contextt Master::ctx = {};

Master::Master(const std::string& ifName, Slove& testSlove) : ifName(ifName), testSlove(testSlove)
{
}

void Master::initialize() const
{
   printf("EtherCAT Startup\n");

   ctx.packedMode = true;

   // Initialize EtherCAT and NIC
   printf("Initializing EtherCAT and NIC\n");
   const int rv = ecx_init(&ctx, ifName.c_str());
   const bool configurationSuccess = rv > 0;
   if (rv <= 0)
      printf("ERROR: Error initializing EtherCAT and NIC\n");

   if (configurationSuccess)
   {
      // Configure network
      printf("Configuring network\n");
      if (ecx_config_init(&ctx) <= 0)
         printf("ERROR: Error configuring network\n");

      if (ctx.slavecount > 0)
      {
         // Mapping slaves onto IOmap
         printf("Mapping slaves onto IOmap\n");
         ec_groupt *group = &ctx.grouplist[0];
         const int ioMapSize = ecx_config_map_group(&ctx, IOmap, 0);
         if (ioMapSize > IOMAP_SIZE)
            printf("ERROR: Error mapping slaves onto IOmap. IOmap size is overflowing supplied buffer\n");

         // Calculate working counter
         expectedWKC = calculateExpectedWorkingCounter();//group->outputsWKC * 2) + group->inputsWKC;
         printf("%d slaves found and configured.\n", ctx.slavecount);
         printf("%d expected working counter.\n", expectedWKC);

         printf("segments : %d : %d %d %d %d\n",
                group->nsegments,
                group->IOsegment[0],
                group->IOsegment[1],
                group->IOsegment[2],
                group->IOsegment[3]);

         /* Configure distributed clocks */
         ecx_configdc(&ctx);

         // Ensure slaves are in SAFE_OP state after mapping
         ecx_statecheck(&ctx, 0, EC_STATE_SAFE_OP, EC_TIMEOUTSTATE * 4);
         if (ctx.slavelist[0].state != EC_STATE_SAFE_OP)
         {
            printf("WARNING: 1 or more slaves not in SAFE_OP after mapping\n");

            // Reading the states of all slaves can refresh the states
            ecx_readstate(&ctx);
            for (int si = 1; si <= ctx.slavecount; si++)
            {
               ec_slavet *slave = &ctx.slavelist[si];
               if (slave->state != EC_STATE_SAFE_OP)
               {
                  printf("Slave %d Alias=%d State=0x%2.2x StatusCode=0x%4.4x : %s\n",
                         si,
                         slave->aliasadr,
                         slave->state,
                         slave->ALstatuscode,
                         ec_ALstatuscode2string(slave->ALstatuscode));
               }
            }
         }
         mappingdone = 1;

         /* Add all CoE slaves to cyclic mailbox handler */
         int sdoslave = -1;
         for (int si = 1; si <= ctx.slavecount; si++)
         {
            ec_slavet *slave = &ctx.slavelist[si];
            if (slave->CoEdetails > 0)
            {
               ecx_slavembxcyclic(&ctx, si);
               sdoslave = si;
               printf(" Slave %d added to cyclic mailbox handler\n", si);
            }
         }

         /* Let network sync to clocks */
         dorun = 1;
         osal_usleep(1000000);

         // Check I/O before entering OP state
         ecx_send_processdata(&ctx);
         ecx_receive_processdata(&ctx, EC_TIMEOUTRET);

         /* Go to operational state */
         printf("Transitioning slaves to OP state\n");
         int checkIterations = 200;
         ctx.slavelist[0].state = EC_STATE_OPERATIONAL;
         ecx_writestate(&ctx, 0);
         //ecx_send_processdata(&ctx);
         //ecx_receive_processdata(&ctx, EC_TIMEOUTRET);
         ecx_statecheck(&ctx, 0, EC_STATE_OPERATIONAL, EC_TIMEOUTSTATE);
         //while (checkIterations-- && ctx.slavelist[0].state != EC_STATE_OPERATIONAL); //TODO is this needed?

         // If slaves are not in OP, print out details
         if (ctx.slavelist[0].state != EC_STATE_OPERATIONAL)
         {
            printf("WARNING: 1 or more slaves not in OP\n");

            // Reading the states of all slaves can refresh the states
            ecx_readstate(&ctx);
            for (int si = 1; si <= ctx.slavecount; si++)
            {
               ec_slavet *slave = &ctx.slavelist[si];
               if (slave->state != EC_STATE_OPERATIONAL)
               {
                  printf("Slave %d Alias=%d State=0x%2.2x StatusCode=0x%4.4x : %s\n",
                         si,
                         slave->aliasadr,
                         slave->state,
                         slave->ALstatuscode,
                         ec_ALstatuscode2string(slave->ALstatuscode));
               }
            }
         }

         // If slaves are in OP, read and print slave inputs and outputs to check they are ok
         else
         {
            int size;

            printf("EtherCAT OP\n");
            inOP = TRUE;

            /* acyclic loop 5000 x 20ms = 100s */
            for (int i = 0; i < 500; i++)
            {
               ecx_send_processdata(&ctx);
               wkc = ecx_receive_processdata(&ctx, EC_TIMEOUTRET);

               printf("Processdata cycle %5d , Wck %3d, DCtime %12" PRId64 ", dt %8" PRId64 ", O:",
                      cycle,
                      wkc,
                      ctx.DCtime,
                      timeerror);

               printf(" Outputs:");
               size = group->Obytes < 8 ? group->Obytes : 8;
               for (int j = 0; j < size; j++)
               {
                  printf(" %2.2x", *(ctx.slavelist[0].outputs + j));
               }

               printf(" Inputs:");
               size = group->Ibytes < 8 ? group->Ibytes : 8;
               for (int j = 0; j < size; j++)
               {
                  printf(" %2.2x", *(ctx.slavelist[0].inputs + j));
               }

               printf("\r");
               fflush(stdout);

               /* Demonstrate SDO access from other threads */
               uint32_t value = 0;
               int size = sizeof(value);
               if (sdoslave > 0)
               {
                  value = 0;
                  int sdo_wkc = ecx_SDOread(&ctx, sdoslave, 0x1018, 0x02, FALSE, &size, &value, EC_TIMEOUTRXM);
                  (void)sdo_wkc;
               }

               osal_usleep(20000);
            }
            printf("\n");
            dorun = 0;
            //inOP = FALSE;
         }

         /* Go to SAFE_OP */
         //printf("EtherCAT to SAFE_OP\n");
         //ctx.slavelist[0].state = EC_STATE_SAFE_OP;
         //ecx_writestate(&ctx, 0);
         //ecx_statecheck(&ctx, 0, EC_STATE_SAFE_OP, EC_TIMEOUTSTATE);

         /* Go to INIT state */
         //printf("EtherCAT to INIT\n");
         //ctx.slavelist[0].state = EC_STATE_INIT;
         //ecx_writestate(&ctx, 0);
         //ecx_statecheck(&ctx, 0, EC_STATE_INIT, EC_TIMEOUTSTATE);
      }
   }
}

/* Cyclic RT EtherCAT thread */
void *Master::ecatthread() // TODO make sure this is proper way to do this
{
   ec_timet ts{};
   int ht;
   static int64_t toff = 0;

   dorun = 1;
   while (!mappingdone)
   {
      osal_usleep(100);
   }
   osal_get_monotonic_time(&ts);
   ht = (ts.tv_nsec / 1000000) + 1; /* round to nearest ms */
   ts.tv_nsec = ht * 1000000;
   ecx_send_processdata(&ctx);

   printf("Starting EtherCAT RT thread \n");
   while (1)
   {
      printf("Currently in EtherCAT RT Loop \n");
      /* calculate next cycle start */
      add_time_ns(&ts, cycletime + toff);
      /* wait to cycle start */
      osal_monotonic_sleep(&ts);
      if (dorun > 0)
      {
         printf("Currently doing EtherCAT RT loop run \n");
         cycle++;
         wkc = ecx_receive_processdata(&ctx, EC_TIMEOUTRET);
         if (wkc != expectedWKC)
         {
            dowkccheck++;
            printf("WARNING : WKC mismatch, expected %d, received %d, total consecutive mm %d\n", expectedWKC, wkc, dowkccheck);
         }
         else
            dowkccheck = 0;

         if (ctx.slavelist[0].hasdc && (wkc > 0))
         {
            /* calculate toff to get linux time and DC synced */
            ec_sync(ctx.DCtime, cycletime, &toff);
         }

         printf("Currently updating slave \n");
         testSlove.update();

         ecx_mbxhandler(&ctx, 0, 4);
         ecx_send_processdata(&ctx);
      }
   }
}

/* Slave error handler */
void *Master::ecatcheck() // TODO make sure this is proper way to do this
{
   printf("Starting EtherCAT check thread \n");

   int slaveix;

   while (1)
   {
      if (inOP && ((dowkccheck > 2) || ctx.grouplist[currentgroup].docheckstate))
      {
         /* one or more slaves are not responding */
         ctx.grouplist[currentgroup].docheckstate = FALSE;
         ecx_readstate(&ctx);
         for (slaveix = 1; slaveix <= ctx.slavecount; slaveix++)
         {
            ec_slavet *slave = &ctx.slavelist[slaveix];

            if ((slave->group == currentgroup) && (slave->state != EC_STATE_OPERATIONAL))
            {
               ctx.grouplist[currentgroup].docheckstate = TRUE;
               if (slave->state == (EC_STATE_SAFE_OP + EC_STATE_ERROR))
               {
                  printf("ERROR : slave %d is in SAFE_OP + ERROR, attempting ack.\n", slaveix);
                  slave->state = (EC_STATE_SAFE_OP + EC_STATE_ACK);
                  ecx_writestate(&ctx, slaveix);
               }
               else if (slave->state == EC_STATE_SAFE_OP)
               {
                  printf("WARNING : slave %d is in SAFE_OP, change to OPERATIONAL.\n", slaveix);
                  slave->state = EC_STATE_OPERATIONAL;
                  if (slave->mbxhandlerstate == ECT_MBXH_LOST) slave->mbxhandlerstate = ECT_MBXH_CYCLIC;
                  ecx_writestate(&ctx, slaveix);
               }
               else if (slave->state > EC_STATE_NONE)
               {
                  if (ecx_reconfig_slave(&ctx, slaveix, EC_TIMEOUTMON) >= EC_STATE_PRE_OP)
                  {
                     slave->islost = FALSE;
                     printf("MESSAGE : slave %d reconfigured\n", slaveix);
                  }
               }
               else if (!slave->islost)
               {
                  /* re-check state */
                  ecx_statecheck(&ctx, slaveix, EC_STATE_OPERATIONAL, EC_TIMEOUTRET);
                  if (slave->state == EC_STATE_NONE)
                  {
                     slave->islost = TRUE;
                     slave->mbxhandlerstate = ECT_MBXH_LOST;
                     /* zero input data for this slave */
                     if (slave->Ibytes)
                     {
                        std::memset(slave->inputs, 0x00, slave->Ibytes);
                     }
                     printf("ERROR : slave %d lost\n", slaveix);
                  }
               }
            }
            if (slave->islost)
            {
               if (slave->state <= EC_STATE_INIT)
               {
                  if (ecx_recover_slave(&ctx, slaveix, EC_TIMEOUTMON))
                  {
                     slave->islost = FALSE;
                     printf("MESSAGE : slave %d recovered\n", slaveix);
                  }
               }
               else
               {
                  slave->islost = FALSE;
                  printf("MESSAGE : slave %d found\n", slaveix);
               }
            }
         }
         if (!ctx.grouplist[currentgroup].docheckstate)
            printf("OK : all slaves resumed OPERATIONAL.\n");
         dowkccheck = 0;
      }
      osal_usleep(10000);
   }
}

void Master::ec_sync(int64 reftime, int64 cycletime, int64 *offsettime)
{
   static int64 integral = 0;
   int64 delta = (reftime - syncoffset) % cycletime;
   if (delta > (cycletime / 2))
   {
      delta = delta - cycletime;
   }
   timeerror = -delta;
   integral += timeerror;
   *offsettime = (int64)((timeerror * pgain) + (integral * igain));
}

void Master::add_time_ns(ec_timet *ts, int64 addtime)
{
   ec_timet addts{};

   addts.tv_nsec = addtime % NSEC_PER_SEC;
   addts.tv_sec = (addtime - addts.tv_nsec) / NSEC_PER_SEC;
   osal_timespecadd(ts, &addts, ts);
}

int Master::calculateExpectedWorkingCounter()
{
   ec_groupt *group = &ctx.grouplist[0];
   return (group->outputsWKC * 2) + group->inputsWKC;
}

int Master::getCurrentWorkingCounter()
{
   return wkc;
}



//TODO
// void checkAndPrintSlaveStates() {
//
// }
