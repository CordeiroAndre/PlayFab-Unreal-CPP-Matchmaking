# PlayFab C++ Matchmaking

In this Repo we got an example on how to make a pretty basic Matchmaking module, using the PlayFab SDK.

### Before start explaining the code...
I am considering that you have already **imported the plugin** into your project, **defined the **Title ID**** of your title in your Plugin Settings, **have authenticated your player before the request**, and have **already registered the queue in PlayFab Dashboard**. 

I really enjoy making my code in modules, and this Repo is one of them. If you want to get to know why, I am adding [this link](https://dev.epicgames.com/community/learning/tutorials/xJ/improving-code-structure-with-unreal-engine-s-c-modules) with the explanation.

## Dependencies
First we need to add our module dependencies. For this module we will be using:

 - "PlayFabCommon", 
 - "PlayFabCpp",
 -  "Core"
 - "Engine" 
 - "CoreUObject"

So, add this  a line like this:

    PublicDependencyModuleNames.AddRange(new []{"Core","CoreUObject", "Engine", "YourProjectName", "PlayFabCommon", "PlayFabCpp"});
   In your *.Build.cs File. 

You will also need a 

## Exposing functions as Async Nodes in Unreal Engine 
When working with PlayFab, most of the features that we are building are exposed to blueprints to be used with widgets.
The JSON Response of the HTTP Requests are desserialized to a model Struct and passed as a delegate with relevant data for us, programmers to use. 
The bindings of these delegates can get pretty messy in blueprints, and as we never want to make Spaghetti code, I strongly suggest you to use ***Blueprint Async Action Base*** to code your PlayFab Features. [This Link](https://nerivec.github.io/old-ue4-wiki/pages/creating-asynchronous-blueprint-nodes.html) shows you how to create it. 

## The Service Class 

In this Repo, the service class is the one responsible for executing the PlayFab Feature that we are implementing (Matchmaking). 
Here, I bind the delegates of PlayFab using BindUObject. [This Link](https://benui.ca/unreal/delegates-advanced/) might help you to understand Delegates if you are not familiar with it.

The following image illustrates the Matchmaking process
 

## Testing 
In order to test, you will need to launch two different game processes, authenticate with two different accounts, and call `UMatchmakingService::MatchmakingService` in another class (or blueprint). 

 - The WorldContextObject Parameter will be automatically assigned by UFUNCTION Macro. 
 - The OwnerID is the returned as Entity ID by the authentication Process.
 -  The QueueName is the queue name you registered in your PlayFab Dashboard.  

 
