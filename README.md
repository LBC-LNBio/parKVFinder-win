Welcome to the **parallel KVFinder for Windows (_parKVFinder-win_)**
[repository](https://github.com/LBC-LNBio/parKVFinder-win), this page was
built to help you get started with our cavity detection software.

# Download & Installation

Follow these steps to install parKVFinder-win on Windows 10:

1. Get the latest release from [here](https://github.com/LBC-LNBio/parKVFinder-win/releases/).

2. Unzip the `parKVFinder-win-x.x.zip` on a preferred location.

3. Add KVFinder_PATH to your environment variables via Command Prompt:

```cmd
:: KVFinder_PATH must be the location of your parKVFinder-win-x.x directory
setx KVFinder_PATH C:\path\to\your\parKVFinder-win-x.x
```

or, open **Control Panel** > **System** > **Advanced system settings** > **Advanced** tab > **Enviroment Variables...** button > **User variables for \<user\>** panel > **New...** button. On the **Edit System Variable** windows, set `KVFinder_PATH` on **Variable name** and `C:\path\to\your\parKVFinder-win-x.x` on **Variable value**.

# PyMOL Plug-in Installation

First, you need to install PyMOL v2.x on your computer. If you do not have it installed, please refer to PyMOL [website](https://pymol.org/2/).

`PyMOL2 parKVFinder Tools` is available to use `parKVFinder-win` with PyMOL v2.x, which has been developed using Qt interface and Python3.

After installing PyMOL v2.x, plug-in requires the installation of [toml](https://pypi.org/project/toml/) module and PyMOL's native Python does not have it installed. So you need to install it:

1. Access PyMOL directory and open `Conda-Prompt.bat` file, which will open a Command Prompt with a conda environment.

2. On this Command Prompt, type:

```cmd
pip3 install toml
```

Finally, to install the `PyMOL2 parKVFinder Tools` on PyMOL v2.x, download the latest `PyMOL2-parKVFinder-Tools.zip` from [here](https://github.com/LBC-LNBio/parKVFinder-win/releases/latest/download/PyMOL2-parKVFinder-Tools.zip) and follow these steps:

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

# Tutorial

For an example of use, there is a comprehensive tutorial for `PyMOL2 parKVFinder Tools` and `parKVFinder-win` command line interface (CLI) [here](https://github.com/LBC-LNBio/parKVFinder/wiki/parKVFinder-Tutorial).

_Note_: Instead of invoking the CLI using `parKVFinder` command, you must use `%KVFinder_PATH%\parKVFinder-win64.exe` command.

# Manual

A comprehensive list of all `PyMOL2 parKVFinder Tools` and `parKVFinder-win` CLI commands are provided in this [manual](https://github.com/LBC-LNBio/parKVFinder/wiki/parKVFinder-Manual).

_Note_: Instead of invoking the CLI using `parKVFinder` command, you must use `%KVFinder_PATH%\parKVFinder-win64.exe` command.

For more information, refer to parKVFinder [wiki](https://github.com/LBC-LNBio/parKVFinder/wiki).

# About

## Issues

parKVFinder software uses GitHub for project management. Please help us by reporting any problems or requests.

On [Issues](https://github.com/LBC-LNBio/parKVFinder-win/issues) page, you can file a:

- [Bug Report](https://github.com/LBC-LNBio/parKVFinder-win/issues/new?assignees=&labels=&template=bug_report.md&title=)
- [Request a new feature](https://github.com/LBC-LNBio/parKVFinder-win/issues/new?assignees=&labels=&template=feature_request.md&title=)

Only make sure the bug or request has not already been reported. Click on “Search” and enter some keywords to search.

Thank you for helping us improve parKVFinder!

## Scientific team

parKVFinder, parKVFinder-win, PyMOL2 parKVFinder Tools and PyMOL parKVFinder Tools were developed by:

- João Victor da Silva Guerra
- Helder Veras Ribeiro Filho
- Leandro Oliveira Bortot
- Rodrigo Vargas Honorato
- José Geraldo de Carvalho Pereira
- Paulo Sergio Lopes de Oliveira

Computational Biology Laboratory ([LBC](https://github.com/LBC-LNBio)), Brazilian Biosciences National Laboratory ([LNBio](https://lnbio.cnpem.br/)), Brazilian Center for Research in Energy and Materials ([CNPEM](https://cnpem.br)).

If you have any further questions, inquires or if you wish to contribute
to parKVFinder project, please contact us at joao.guerra@lnbio.cnpem.br and paulo.oliveira@lnbio.cnpem.br.

## Citing parKVFinder

João Victor da Silva Guerra, Helder Veras Ribeiro Filho, Leandro Oliveira Bortot, Rodrigo Vargas Honorato, José Geraldo de Carvalho Pereira, Paulo Sergio Lopes de Oliveira, ParKVFinder: A thread-level parallel approach in biomolecular cavity detection, SoftwareX, 2020, [https://doi.org/10.1016/j.softx.2020.100606](https://doi.org/10.1016/j.softx.2020.100606).

## Funding

This work was supported by the Fundação de Amparo à Pesquisa do Estado de São Paulo (FAPESP) [Grant Number 2018/00629-0], Conselho Nacional de Desenvolvimento Científico e Tecnológico (CNPq) [Grant Number 350244/2020-0], and Brazilian Center for Research in Energy and Materials (CNPEM).

## License

The software is licensed under the terms of the GNU General Public License version 3 (GPL3) and is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
