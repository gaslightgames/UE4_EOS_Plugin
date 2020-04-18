# UE4_EOS_Plugin
A plugin and sample project, built for Unreal Engine 4, that implements the Epic Online Services SDK

This plugin does not include the EOS (Epic Online Services) SDK.  You will have to register and download this separately.  Then copy the Bin, Include and Lib folders to the Plugins/Source/ThirdParty directory.
The project currently supports SDK v1.5
Make sure to right click the .uproject, Generate Visual Studio files and compile.

If you download and run the sample project, you will need to:
- Have registered your project on the EOS site (Dev Portal)
- Retrieved your ProductId, SandboxId and DepolymentId
- Add the ProductId, SandboxId and DeploymentId's to the Settings in Project Settings -> UEOS

In the sample, within the Player Controller, there are several input events connected to simple actions.
- E will attempt to initialise the EOS SDK, passing your parameters.
- S will attempt to shutdown the EOS SDK.

(Obviously you will want/need to move this to something more appropriate for your project.)

Once you have the SDK initialized, then you can use:
- B will initialize a Metric.
- N will attempt to Begin a Player Session.
- M will attempt to End a Player Session.

The options for Account Login/Logout will **ONLY** work if your Project is on the Epic Store, or an Epic Store Partner. The same goes for running the Developer Authentication Tool.
- ~~I will attempt to Login~~
- ~~O will attempt to Logout (will only work if a successful Login occurred first)~~

The project, blueprint nodes and comments have more direct usage information.

License:
Provided "as is."  So feel free to use it in any and all of your own projects.  Use it as a "jumping off point" to extend, fix and included into anything else you want.
(I only ask that, if you do find it useful and fix/add something - please consider adding back to the plugin with a pull request!)
