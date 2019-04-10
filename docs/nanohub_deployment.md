---
title: "Deploying Apps on nanoHUB"
keywords: Engineered nanoBIO, NCN, nanoHUB
topnav: topnav
hide_sidebar: true
summary: This is a quick cheat sheet based on recent experiences and is by no means a substitute to any of nanoHUB documentations. Use this for quick reference but refer to official documentation for detailed guidance. 
---

nanoHUB facilitates multiple options to enable user interactivity with the simulation tools (apps). The following steps are particularly tuned for NSF-funded NCN nodes to deploy tools to nanoHUB Cyber Platform. The steps outlined illustrate one particular approach. These may or may not apply to your use case. Contact nanoHUB support for appropriate instructions to your use case. 

## Set up Workspace
* You need to create “affiliated institution” accounts and use your university credentials to sign in to your nanoHUB account.
* Tool developers can then submit a ticket to request workspace (Linux desktop in a browser) access
 using the HELP button at the top of any nanoHUB page.

## Change the Project Directory Structure
*  You should convert your project to the following directory structure.

### A typical top-level directory layout

    .
    ├── bin					# Executables will be installed here
    ├── docs				# Documentation files
    ├── examples			# Example test programs or result files
    ├── middleware			# NanoHub-Rappture specific folder where invoke file is kept
    ├── scripts				# Script files for cloud/cluster platforms
    ├── src					# Source files of your project
	├── src/preprocessing	# Source files of preprocessing project
	├── src/postprocessing	# Source files of postprocessing project
    └── README.md			# Readme file of your project  

## Build Instructions - Makefile
* You need to create a Makefile to bulid and install the project. This Makefile is kept inside the src folder. Example Makefile is provided below but you need to modify this to match the library requirements of your program.


### Executing the application
* Log in to nanoHUB account and launch workspace tool. This will provide you a remote login to a nanoHUB VM.
* We recommend hosting your project as a GitHub open-source repository. git clone your project in to a directory in your workspace.
* Go to np-assembly-lab/src directory and (cd np-assembly-lab/src/)
* You should provide the following make command to make the project. This will create the executable and Install the executable into the np-assembly-lab/bin directory.
    * make nanoHUB-install
* Then go back to home directory (np-assembly-lab/) using “cd ..” command.
* Now you are ready to run the program with nanoHUB rapture framework using following command :
    * ./middleware/invoke
* This will start a GUI where you may change the parameters (physical, computing) as desired, and click Simulate button on the top right of the GUI.
* Once the simulation has finished, density profiles will be plotted on the right hand side of the GUI.
* You can monitor changes to your GUI as you modify the code via the above procedure by updating the git project.

