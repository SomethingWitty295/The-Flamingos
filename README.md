# Flamingo Centered Data-Distribution-Service

***NOTE: This is a rougly translated version of the FCD Manual. If you'd like a more official and organized view of this document, click the link below.***

https://drive.google.com/file/d/15G9rLGYkvGxbv9oNscU7B5Av8_mcZ64b/view?usp=sharing

# General Info

1.  Our DDS provides two distinct applications for communicating data between different systems terminals. The publisher and subscriber applications send and receive “Flamingo” structures of data to each other respectively. These two applications follow a titled “FlamingoWare” API standard, in which OpenDDS library operations are packed in “Flock” structures which handle communication for the user.
2.  Communication between applications follow conventional DDS conventions, reaching each other through specific Domains (specified by ID), and topics (specified by name) inside of domains.
3.  Users interface with each application through a command prompt window, in which they can control data to send out to specific domains/topics.
4.  The DDS follows “Flamingo” structures, defined in a shared Interface Definition Language (IDL). These structures send date, time, subject, among others.
5. Capable developer mode including minimal logging of OpenDDS activity throughout the FlamingoWare.

***Example screenshot of a Flamingo received by the Subscriber view.***
**![](https://lh3.googleusercontent.com/ylIBvGTUTJaTJrOD2Bt7rZ-SNXKcGckA4TswhYEJ-NFYbkp16nLMARU6uvLmESYEXPZ1A6jk6sZGA1uUcE8T-3-7Mws0hx57yPiebZvPEW4p12ELpmc4Jc7Uh-O7_2Hdt94AXzps)**

# Service Description
    

### Middleware API Description
    

![](https://lh3.googleusercontent.com/vmfaOBj1qvbA6Ng7OQMbM_SaYAfSMDmJODWhqWAtWXEqHd4V1_Y1mESCSxUZ4n4IjCyEnfWOGR4ZbluiPFrK0DQUg_Ghq7AnFdaq8W0Mus5EOACLkZg4NN1TWF8e-aOVIqoQQepc)

Here is a simple physical representation of the FCD system.

This is the DDS system described in this document. The structure is strictly of the FCD system. For OpenDDS, refer to 1.2’s OpenDDS Developer Guide for a diagram of the OpenDDS structure.

This service consists of two separate applications (Publisher and Subscriber) which both refer to the API set by the FlamingoWare to send/receive data properly. These applications must abide by both the Flamingo data structures and the Flock API standards for data transmission to correctly complete.

### Project Setup
![](https://lh6.googleusercontent.com/2vuy3XxzFzgLtfc5jko9CTEBYowr9GB8QB5iY9Ni_i9pCqjahci_NFuZ2iNJ6VTBlCgpju_ga_0xX5dkHU5eT_xdOHrTknrXWUD0J8aUGudSgk-_aBuN2br14l4923GpTdoQ3xgX)
As mentioned previously, the FCD consists primarily of OpenDDS libraries that handle the deeper middleware data transmission. Although interference with OpenDDS entities is irrelevant when using the FlamingoWare(mostly), a basic understanding of the structure of OpenDDS is helpful for users. Here is a slice of a diagram taken from the official OpenDDS Developer’s Guide, detailing the scope of domains and topics.

  

### API Visualization
    

![](https://lh6.googleusercontent.com/91p94bw0qcKjuWHxt4b6FHNgwMQxme5uByrYfeA6DKOKoVOKn4SD0iFan2JFp2Zm4cyesJkkb-zGuJiD2HUzkutRYH0JRhhL14wvNtasOUmkfF6s8f0Z9fthnC51ATjdQYBRgBJJ)

The FlamingoWare standard API makes use of Flock structures. There are two Flock structures for use in the Publisher and Subscriber classes respectively. These two contain essential OpenDDS entities, but there are four variables that the user must instantiate before use in both types; three of which are shared by both types of Flocks.

domainID: domain that the Publisher/Subscriber will be active in corresponding by ID number.
topicName: topic that the Publisher/Subscriber will be active in corresponding by name.
dpf: Only OpenDDS entity required by user to be instantiated with. More info in SECTIONHERE.

One Publisher exclusive variable:
flamingo: The Flamingo structure containing the data that will be sent to any subscriber.

## Running the Generic Default Flamingo Example
Visual Studio & Perl Setup
The preferred method of building Publisher and Subscriber applications with our FlamingoWare is via a .sln file. This .sln file must be processed with a perl script, as follows:
1.  Open a command prompt in a visual studio environment.
2.  Navigate to the %DDS_ROOT% directory.
3.  run setenv.cmd
4.  Navigate to the %DDS_ROOT%\The-Flamingos\src directory.
5.  run the following command: perl %ACE_ROOT%\bin\mwc.pl -type vs2019 Flamingo.mwc
6.  Previous step should have produced the appropriate .sln file.

The user can modify the flamingo.mwc file to include other .cpp relevant files for any other project dependent classes.

From here, the user should launch the respective .sln file in their Visual Studio Environment by using the command devenv. From there, generically building from Visual Studio should generate the appropriate executables.

### Relevant File Information

Included in the src directory is all the appropriate/required type support files, header files, C++ files, certain helpful scripts, among others. There are only a few that are noteworthy for running the generic FCD program.

devmode.cmd - simple command line script that will set the appropriate environment variable for launching either application in development mode.

rtps.ini - configuration file required when launching either publisher or subscriber.

publisher.exe and subscriber.exe - both publisher and subscriber executable programs respectively.

### Run programs with proper arguments

From here, testing with RTPS will require two terminal windows. These windows must have set relevant OpenDDS environment variables with the script found at (%DDS_ROOT%\setenv.cmd).
For both applications, the arguments you should give will be: -DCPSConfigFile rtps.ini
This will ensure that our applications correctly follow RTPS (instead of DCPS).
When launched, either application will display a Flamingo ASCII for the respective app, and then ask you for appropriate information for setting up in the service.

### Using the publisher

![](https://lh3.googleusercontent.com/WGZk1U0ik1qGkiHMgGxFtubgnINMfjJjkbt5-Ao7ijMfk4BW63zKxiz4EwsZ_V4C41oAWKyMwyxPsqCaxML_UyNXwrc41iXr1NxnlXLx_eBx2_mUU-qSYZ6P6EUDFbms9o6o47dH)

Here is a sample screenshot of the process of launching the Publisher side.

As you can see, the main control will display whatever your current status is in your Publisher(via the PubFlock and Flamingo). It displays the current domainID and topicName, as well as details relating to the Flamingo: username, subject, and generic data.

The publisher has a couple options at the users’ disposal. We will go in detail for each submenu available.
s: Try to send your data.
From here, your terminal will communicate if the Flamingo is still trying to be sent. If the Flamingo fails to send (after a 10 second attempt), then it will notify the user. When it properly sends to Subscribers, it will notify the user before bringing them back to the main control interface.

u: set username. and c: change your Flamingo to send.
These two will allow you to either change your username permanently (which is reflected in the Flamingos you send) and change the Flamingo data you will send (subject and generic data) respectively.

e: exit.
This will properly cleanup any service activity that is still active, and close the application. It is important for a user to properly exit with the exit command within the application, as it is vital to the functionality of other users possibly receiving Flamingos.

### Using the subscriber

![](https://lh6.googleusercontent.com/5TTp9eIUC3OTzz82aYAFOVoNpBqRGd-hvohGZbKfsxDOHryJTYVVZ6DPn3oByGjD7e2f6w5Adowf1kZBDLDQ-tblgaaYwCiIfUR0fjTuWEyrMagvYBfraJIiwib1rb14LwQSBYps)

Here is a sample screenshot of the process of launching the Subscriber side.

The subscriber has much less options than the publisher; in fact, it doesn’t have any. The most important information available is the domainID and topicName.

While on the main control, you are open to any and all data being sent to the subscribers’ designated topic and domain from any publishers. The Flock that is created in the subscriber is active at all times. Without getting too in depth on the lower level view, the DataReader in the SubFlock is always detecting action in the contextual domain and topic. If anything is properly sent, you’ll see it.

NOTE: The main control view will not be updated to view once Flamingos are properly received.

### Developer Mode
Setting the environment variable FLAMINGO_DEV will enable the user of the relevant terminal to run either application in ‘Dev Mode’. If you’d like to easily enable it, you can run devmode.cmd to set it for you.

Developer mode has only one debug mode, which will update the terminal with relevant OpenDDS middleware confirmation of certain services being properly registered.
