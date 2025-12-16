//
// Created by srfas on 12/13/2025.
//

#include "Master.h"

#include <cinttypes>

Master::Master(const std::string& ifName) : ifName(ifName) {
}


void Master::initialize() {
   printf("EtherCAT Startup\n");

   ctx.packedMode = true;

   // Initialize EtherCAT and NIC
   printf("Initializing EtherCAT and NIC");
   const int rv = ecx_init(&ctx, ifName.c_str());
   const bool configurationSuccess = rv > 0;
   if (rv <= 0)
      printf("ERROR: Error initializing EtherCAT and NIC");

   if (configurationSuccess)
   {
      // Configure network
      printf("Configuring network");
      if (ecx_config_init(&ctx) <= 0)
         printf("ERROR: Error configuring network");

      if (ctx.slavecount > 0)
      {
         // Mapping slaves onto IOmap
         printf("Mapping slaves onto IOmap");
         ec_groupt *group = &ctx.grouplist[0];
         const int ioMapSize = ecx_config_map_group(&ctx, IOmap, 0);
         if (ioMapSize > IOMAP_SIZE)
            printf("ERROR: Error mapping slaves onto IOmap. IOmap size is overflowing supplied buffer");

         // Calculate working counter
         expectedWKC = (group->outputsWKC * 2) + group->inputsWKC;
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
            printf("WARNING: 1 or more slaves not in SAFE_OP after mapping");

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

         /* Go to operational state */
         printf("Transitioning slaves to OP state");
         int checkIterations = 200;
         ctx.slavelist[0].state = EC_STATE_OPERATIONAL;
         ecx_writestate(&ctx, 0);
         //ecx_send_processdata(&ctx);
         //ecx_receive_processdata(&ctx, EC_TIMEOUTRET);
         ecx_statecheck(&ctx, 0, EC_STATE_OPERATIONAL, EC_TIMEOUTSTATE);
         while (checkIterations-- && ctx.slavelist[0].state != EC_STATE_OPERATIONAL); //TODO is this needed?

         // If slaves are not in OP, print out details
         if (ctx.slavelist[0].state != EC_STATE_OPERATIONAL)
         {
            printf("WARNING: 1 or more slaves not in OP");

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
            for (int i = 0; i < 5000; i++)
            {
               printf("Processdata cycle %5d , Wck %3d, DCtime %12" PRId64 ", dt %8" PRId64 ", O:",
                      cycle,
                      wkc,
                      ctx.DCtime,
                      timeerror);

               size = group->Obytes < 8 ? group->Obytes : 8;
               for (int j = 0; j < size; j++)
               {
                  printf(" %2.2x", *(ctx.slavelist[0].outputs + j));
               }

               printf(" I:");
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
            inOP = FALSE;
         }

         /* Go to SAFE_OP */
         printf("EtherCAT to SAFE_OP\n");
         ctx.slavelist[0].state = EC_STATE_SAFE_OP;
         ecx_writestate(&ctx, 0);
         ecx_statecheck(&ctx, 0, EC_STATE_SAFE_OP, EC_TIMEOUTSTATE);

         /* Go to INIT state */
         printf("EtherCAT to INIT\n");
         ctx.slavelist[0].state = EC_STATE_INIT;
         ecx_writestate(&ctx, 0);
         ecx_statecheck(&ctx, 0, EC_STATE_INIT, EC_TIMEOUTSTATE);
      }
   }
}

//TODO
// void checkAndPrintSlaveStates() {
//
// }