# Build and run Tetris

## Source 
The folder named [Tetris](Tetris) contains the source code for the project.

### Souce Listing

The folder named [source](source) contains only the basic source files and should be used if you intend to follow the guide.
 
 - AllegroFont.cpp
 - AllegroFont.h
 - AllegroResources.cpp
 - AllegroResources.h
 - AllegroSound.cpp
 - AllegroSound.h
 - BasicResources.cpp
 - BasicResources.h
 - FpsCounter.cpp
 - FpsCounter.h
 - GameAudio.cpp
 - GameAudio.h
 - GameFonts.cpp
 - GameFonts.h
 - GameMonitor.cpp
 - GameMonitor.h
 - Main.cpp
 - Main.h
 - Resource.h
 - Scope.cpp
 - Scope.h
 - stdafx.cpp
 - stdafx.h
 - targetver.h
 - Tetris.rc
 - TetrisElement.cpp
 - TetrisElement.h
 - TetrisGame.cpp
 - TetrisGame.h
 - TetrisKey.cpp
 - TetrisKey.h
 - TetrisKeyboard.cpp
 - TetrisKeyboard.h
 - TetrisPlayer.cpp
 - TetrisPlayer.h
 - TetrisShape.cpp
 - TetrisShape.h
 - Util.cpp
 - Util.h
 
 Download only these files to avoid insecure binaries.

## Notes
The game is written in C++ and uses Allegro 5.
We use Visual Studio 2019 to complie and run the program.
When we use the term Allegro we mean Allegro 5.

## Create the Projct
Start Visual Studio and create a new project.

![Image](img/StartVisualStudio2019.png)

Select C++ and select Windows Desktop Application.

![Image](img/CreateProject.png)

Name the project.

![Image](img/NameProject.png)

Create the project.

![Image](img/ProjectCreated.png)

Confirm any prompt that appears.

## Add Allegro
Start the NuGet package manager.

![Image](img/OpenNuget.png)

Click Browse when NuGet has opened.

![Image](img/ClickBrowse.png)

Type allegro in the search field then select "Allegro by Allegro Developers v5", mark the checkbox and click install.

![Image](img/SearchAndInstall.png)


Installing allegro may take some time. You might be asked to confirm the installation.

![Image](img/WaitForInstall.png)

Wait until the installation process finishes as shown in the output window.

## Configure Allegro
View the properties of the project. Right click the project and selecting Properties.

![Image](img/ProjectProperties.png)

Select "All Configurations" in the top left corner and click "Allegro 5" then click "Add-ons". Then add the features of allegro that we intend to use.

![Image](img/SelectAddOns.png)

Select "Release" in the top left corner and click "Allegro 5" then click "Library Type".  Let the release version be a single executable.

![Image](img/SingleExecutable.png)

Optionally configure the Debug version by selecting "Debug" instead of "Release".

You might also want to disable the linker warning about missing .pdb files

![Image](img/DisableLinkerWarning.png)

## Copy the [source](Tetris) code
You can use the command line to copy the source files to the project folder or the guide below.

Open File Explorer and navigate to the source folder and copy all the files in it.

![Image](img/SelectFilesToCopy.png)

Then close the File Explorer.

Right click the project and select 'Open Folder in File Explorer'.

![Image](img/OpenExplorer.png)

It should look like this.

![Image](img/ExplorerOpened.png)

Then paste the [source](Tetris) files with Ctrl-V and overwrite existing files.

![Image](img/ReplaceOrSkip.png)

Confirm if Visual Studio asks you to reload any files.

Then close the File Explorer.

## Configure the project to include the source files
Select 'Project -> Add Existing Item' and add the files we just pasted.

![Image](img/AddExisting.png)

Here you can see the files to avoid.

![Image](img/FilesCopied.png)    

Delete the file Tetris.cpp from the project by right clicking it.

![Image](img/DeleteTetrisCpp.png)

The project should now compile.

## Run the game
Press F5 to build and run the game.

![Image](img/RunGame.png)

General key bindings.

            - M      Togle music.
            - F3     Togle grid.
            - F5     Start a new game.
            - F10    Togle the way the pieces move.
Player 1 key bindings.

            - A      Move Left.
            - S      Move Down.
            - D      Move Right.
            - W      Rotate.
Player 2 key bindings.

            - LEFT   Move Left.
            - DN     Move Down.
            - RIGHT  Move Right.
            - UP     Rotate.

## Comments
The game needs better keyboard handling, ie keyboard strokes should be stored in a queue with all SHIFT info.

Rotation needs to be made less strict and refactored with they keyboard code.

The screen should shake proportionaly to the amount of rows popped.

Maybe enable dropping of 1x1 piece of debris on the opponents screen, that cant be moved, if you clear rows twice in a row.

Maybe a 1x1 piece can begin to grow, and pop of the player drops a piece on it before it is fully grown.

Create a version that uses windows native sounds only.

