# Installation

First, you need to install PyMOL v2.x on your computer. If you do not have it installed, please refer to PyMOL [website](https://pymol.org/2/).

`PyMOL2 parKVFinder Tools` is available to use parKVFinder-win with PyMOL v2.x, which has been developed using Qt interface and Python3.

After installing PyMOL v2.x, plug-in requires the installation of [toml](https://pypi.org/project/toml/) module and PyMOL's native Python do not have it installed. So you need to install it:

1. Access PyMOL directory and open `Conda-Prompt.bat` file, which will open a Command Prompt with a conda environment.

2. On this Command Prompt, type:

```cmd
pip3 install toml
```

2. Download the latest release of PyMOL2 parKVFinder Tools from [here](https://github.com/LBC-LNBio/parKVFinder/releases).

1. Open PyMOL.
2. Go to **Plugin** menu, click on **Plugin Manager**.
3. The **Plugin Manager** window will open, go to the **Install New Plugin** tab.
4. Under **Install from local file** panel, click on **Choose file...**.
5. The **Install Plugin** window will open, select the `PyMOL2-parKVFinder-Tools.zip` that you downloaded earliar.
6. The **Select plugin directory** window will open, select
   **C:\\\<user\>\\path\\to\\PyMOL\\lib\\site-packages\\pmg_tk\\startup** and click **OK**.
7. The **Confirm** window will open, click **OK**.
8. The **Sucess** window will appear, confirming that the plug-in has
   been installed.
9. Restart PyMOL.
10. `PyMOL2 parKVFinder Tools` is ready to use.

Or, instead of selecting `PyMOL2-parKVFinder-Tools.zip` (Step 5), you can select `__init__.py` file on `parKVFinder-win-x.x\tools\PyMOL2-parKVFinder-Tools` directory tree.
