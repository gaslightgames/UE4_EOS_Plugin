# UE4_EOS_Plugin
A plugin and sample project, built for Unreal Engine 4, that implements the Epic Online Services SDK

This plugin does not include the EOS (Epic Online Services) SDK.  You will have to register and download this separately.  Then copy the Bin, Include and Lib folders to the Plugins/Source/ThirdParty/EOSSDK directory.

The project currently supports SDK **v1.6.2** and Engine version **4.25.1**.
Additional support for Engine version 4.24 is now available.

Make sure to right click the .uproject, Generate Visual Studio files and **compile**.

If you download and run the sample project, you will need to:
- Have registered your project on the EOS site (Dev Portal)
- Retrieved your ProductId, SandboxId, DepolymentId, ClientId and Client Secret
- Add the ProductId, SandboxId, DeploymentId, ClientId and Client Secret to **DefaultEngine.ini** (see the appropriate area)

The project will now auto-initialize (and Shutdown) the SDK on Module startup.

The current version only has support for logging **IN** to an account. You will require a **ClientId** and **Client Secret**. AND right now only using the **Dev Tool** (https://dev.epicgames.com/docs/services/en-US/DeveloperAuthenticationTool/index.html) has been tested and logging in (see Log output where "Status: 2" is given).

The project has now moved over to using an Online Subsystem, so in your own project, you will need to add the appropriate sections and details to your own DefaultEngine.ini. See the sample project, EOSBasic, for more implementation details.

License:
Provided "as is."  So feel free to use it in any and all of your own projects.  Use it as a "jumping off point" to extend, fix and included into anything else you want.
(I only ask that, if you do find it useful and fix/add something - please consider adding back to the plugin with a pull request!)
