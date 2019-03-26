# UE4_EOS_Plugin
A plugin and sample project, built for Unreal Engine 4, that implements the Epic Online Services SDK

This plugin does not include the EOS (Epic Online Services) SDK.  You will have to register and download this separately.  Then copy the Bin, Include and Lib folders to the ThirdParty directory.
Make sure to right click the .uproject, Generate Visual Studio files and compile.

If you download and run the sample project, you will need to:
- Have registered your project on the EOS site
- Retrieved your ProductId and SandboxId
- Add the ProductId and SandboxId's to the Settings in Project Settings -> UEOS

In the sample, within the Player Controller, there are two events connected to simple actions.  Currently 'E' and 'S'.
E will attempt to initialise the EOS SDK, passing your parameters.
S will attempt to shutdown the EOS SDK.
Obviously you will want/need to move this to something more appropriate for your project.
