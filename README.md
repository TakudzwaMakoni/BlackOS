
Projects like these help to keep me busy, because i have to do a little research, try new languages/features/packages, plan how im going to implement it, think of a name and make the logo etc. In short it's a very effective distraction. I mention that to clarify that this isnt by any means meant to be professional software developed for purpose by an experienced Engineer or Computer Scientist. The GPL v3 license may state it more explicitly:


>  This Program is free software: you can redistribute
>  it and/or modify it under the terms of the GNU General Public
>  License as published by the Free Software Foundation, either
>  version 3 of the License, or (at your option) any later version.
> 
>  This Program is distributed in the hope that it will
>  be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
>  of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
>  GNU General Public License for more details.
> 
>  You should have received a copy of the GNU General Public License
>  along with This Program. If not, see <http://www.gnu.org/licenses/>.

That being said, i do attempt to use the best practices possible with the knowledge and resources at my disposal, and i would welcome a contributor, or a constructive code review.

BlackOS is currently comprised of the Trinkets shell and the DisplayKernel library detailed below, there are other projects that i want to entertain in the future which i hope to add to ODH. That aside, there are many features and fixes to be added to the existing projects.

Every once in a while i get an idea i want to try in some language. I had started calling them silly things like "Operation Dark Horse", which is really just the name handle for "cool stuff you can do in the terminal with C++". Operation Dark Horse is the successor to Operation FreedomFighter (OFF), the name handle for "cool stuff you can do in the terminal with python". Originally i would just call projects freedomfighter and dark horse, though i started prepending "operation" to them, it will surely motivate me to actually try the ideas i come up with instead of never getting around to it (Now i use the code to distract myself from other important things), as well as it being my continued contribution to the ongoing commitment to lighten up. 

![](media/TrLogo.png)

# ScreenShell

Trinkets ScreenShell (C) is a shell which uses the DisplayKernel API to develop a powerful shellsurfing UI and a feature rich Terminal Shell superposed onto the default shell. It is based on an earler project which was essentially a few bash and python shortcuts and tools while using the terminal. I hope to develop an equivalent WindowShell which can be used to create a subwindow shell, and an even lighter LineShell, which allows commands to be executed from within an entirely different BlackOS program. My intention is that the three implementations form as a whole the Trinkets SuperShell project.

## starting the shell
After building from source, you can start the Trinkets Screenshell by running the binary Tr in Framework/Trinkets/ScreenShell.
Instead of moving this binary to bin in the PATH folder, i have made an alias for the binary in the build directory in the shell config file (e.g. for bash it is ~/.bashrc).
![](media/start-tr.gif)

## list children in parent directory
The built in commands are named after their corresponding unix-like equivalent, including also optional flags. The ls command includes the option -a to show hidden files. It displays an ordered list of files in the given directory (the current directory as default if no directory parameter is entered), with information about each file.
![](media/tr-ls.gif)

## navigate the filesystem with 'nd' 
the built in command nd opens a UI window so that the user can navigate the filesystem, starting either at the current or some given directory, with option -a to show hidden files. When activated, the user can view directory contents, jump in and out of files using an editor, and change to a selected directory.
![](media/nd-cd.gif)

### editing files with 'nd' 
![](media/nd-edit-files.gif)

### switch on 'show hidden items' option 
![](media/nd-hidden.gif)

### large number of files are automatically paginated
![](media/nd-paginate.gif)

## shortcut to directories
sc is a built in command that opens a window containing a list of shortcuts and their paths, so that the user can quickly jump to that directory. Shortcuts can be added or removed by editing the file in home/user/.tr/shortcuts.txt
![](media/sc-add.gif)

## customise and configure the shell
The shell can be customised by editing the file /home/user/.tr/config.txt. The user can map keys to actions, can change the theme and appearance of the shell. These configurations will be loaded on startup of the shell.
![](media/configure-on-startup.gif)

### changing themes 
![](media/change-themes.gif)

![](media/DisplayKernelLogo.png)

The DisplayKernel (C) API features Methods for creating c++ programs with a terminal-based User Interface. The DisplayKernel library borrows methods from the ncurses library to create window objects like menu systems, grids and canvassess and ways to modify them. 

