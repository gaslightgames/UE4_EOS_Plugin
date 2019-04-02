# UE4_EOS_Plugin
A plugin and sample project, built for Unreal Engine 4, that implements the Epic Online Services SDK

This plugin does not include the EOS (Epic Online Services) SDK.  You will have to register and download this separately.  Then copy the Bin, Include and Lib folders to the ThirdParty directory.
Make sure to right click the .uproject, Generate Visual Studio files and compile.

If you download and run the sample project, you will need to:
- Have registered your project on the EOS site
- Retrieved your ProductId and SandboxId
- Add the ProductId and SandboxId's to the Settings in Project Settings -> UEOS

In the sample, within the Player Controller, there are several input events connected to simple actions.
E will attempt to initialise the EOS SDK, passing your parameters.
S will attempt to shutdown the EOS SDK.

(Obviously you will want/need to move this to something more appropriate for your project.)

Once you have the SDK initialized, then you can use:
B will initialize a Metric.
N will attempt to Begin a Player Session.
M will attempt to End a Player Session.

The project, blueprint nodes and comments have more direct usage information.
