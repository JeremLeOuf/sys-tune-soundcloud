#define TESLA_INIT_IMPL
#include "tune.h"
#include "gui_error.hpp"
#include "gui_main.hpp"
#include "sdmc/sdmc.hpp"
#include "pm/pm.hpp"
#include "config/config.hpp"

#include <tesla.hpp>

class SysTuneOverlay final : public tsl::Overlay {
  private:
    const char *msg = nullptr;
    Result fail     = 0;

  public:
    void initServices() override {
        // Step 1: Initialize PM service
        Result rc = pm::Initialize();
        if (R_FAILED(rc)) {
            this->msg = "Step 1 FAILED:\npm::Initialize()";
            this->fail = rc;
            return;
        }

        // Step 2: Try to connect to sys-tune service
        rc = tuneInitialize();
        
        // If service not found, try to launch sysmodule
        if (R_VALUE(rc) == KERNELRESULT(NotFound) || R_VALUE(rc) == KERNELRESULT(ConnectionRefused)) {
            u64 pid = 0;
            const NcmProgramLocation programLocation{
                .program_id = 0x4200000000000000,
                .storageID  = NcmStorageId_None,
            };
            
            rc = pmshellInitialize();
            if (R_FAILED(rc)) {
                this->msg = "Step 2a FAILED:\npmshellInitialize()";
                this->fail = rc;
                return;
            }
            
            rc = pmshellLaunchProgram(0, &programLocation, &pid);
            pmshellExit();
            
            if (R_FAILED(rc)) {
                this->msg = "Step 2b FAILED:\npmshellLaunchProgram()";
                this->fail = rc;
                return;
            }
            
            if (pid == 0) {
                this->msg = "Step 2c FAILED:\npid = 0 after launch";
                this->fail = 0x1234; // Custom error code
                return;
            }
            
            // Wait for service to start
            svcSleepThread(500'000'000ULL);
            rc = tuneInitialize();
        }

        // Step 3: Check final connection
        if (R_FAILED(rc)) {
            this->msg = "Step 3 FAILED:\ntuneInitialize() after launch";
            this->fail = rc;
            return;
        }

        // Step 4: Initialize SDMC
        if (R_FAILED(sdmc::Open())) {
            this->msg = "Step 4 FAILED:\nsdmc::Open()";
            return;
        }

        // Step 5: Check API version
        u32 api;
        rc = tuneGetApiVersion(&api);
        if (R_FAILED(rc)) {
            this->msg = "Step 5a FAILED:\ntuneGetApiVersion()";
            this->fail = rc;
            return;
        }
        
        if (api != 2) {  // TUNE_API_VERSION
            this->msg = "Step 5b FAILED:\nAPI version mismatch";
            return;
        }
        
        // If we get here, everything worked!
        // this->msg remains nullptr for success
    }

    void exitServices() override {
        sdmc::Close();
        pm::Exit();
        tuneExit();
    }

    std::unique_ptr<tsl::Gui> loadInitialGui() override {
        if (this->msg) {
            return std::make_unique<ErrorGui>(this->msg, this->fail);
        } else {
            return std::make_unique<MainGui>();
        }
    }
};

int main(int argc, char **argv) {
    return tsl::loop<SysTuneOverlay>(argc, argv);
}
