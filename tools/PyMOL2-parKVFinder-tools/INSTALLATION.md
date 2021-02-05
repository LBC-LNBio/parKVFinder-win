# Guide for developers

Welcome to PyMOL KVFinder-web Tools guide, which aims to aid developers with relevant information about its operation.

## Summary

1. Download & Installation
2. PyMOL Installation
    - Linux
    - MacOS
3. Code Description
    - Threads
    - Classes
    - HTTP Responses

---

## Download & Installation

[PyMOL v2](https://pymol.org/2/) is required if you wish to use PyMOL KVFinder-web Tools. If necessary, refer to this [section](#PyMOL\ Installation) for installing PyMOL.

Follow these steps to install PyMOL KVFinder-web Tools:

1. Install the necessary Python modules from [requirements.txt](https://github.com/jvsguerra/kvfinder-ws/releases/download/v0.1/requirements.txt) file.

```bash
$ pip3 install -r requirements.txt
```

2. Download the latest release of PyMOL KVFinder-web Tools from [here](https://github.com/jvsguerra/kvfinder-ws/releases/download/v0.1/PyMOL-KVFinder-web-tools.zip).

    1. Open PyMOL;
    2. Go to **Plugin** menu and select **Plugin Manager** option;
    3. The **Plugin Manager** window will open, go to the **Install New Plugin** tab;
    4. Under **Install from local file** group, click on **Choose file...**;
    5. The **Install Plugin** window will open, select the `PyMOL-KVFinder-web-Tools.zip`;
    6. The **Select plugin directory** window will open, select `/home/user/.pymol/startup` and click **OK**;
    7. The **Confirm** window will open, click on **OK**;
    8. The **Sucess** window will open, confirming that the plugin has been installed;
    9. Restart PyMOL;
    10. **PyMOL KVFinder-web Tools** is ready to use under **Plugin** menu.

Or, if you clone this [repository](https://github.com/jvsguerra/kvfinder-ws), instead of selecting PyMOL-KVFinder-web-Tools.zip (Step v), user must select `__init__.py` of PyMOL-KVFinder-web-Tools directory

---

## PyMOL Installation

The installation procedures for PyMOL are different for Linux and macOS.

### Linux

The easiest way is to use your distribution package manager.

```bash
$ sudo apt install pymol
```
_Note_: PyMOL version varies according to your distribution version.

Or, you can install PyMOL through [Anaconda](https://www.anaconda.com/)
package management system.

Follow these steps:

**1.** Create a new environment and activate it: 
```bash
$ conda create --name pymol2 python=3.7
$ conda activate pymol2
```

**2.** Install PyMOL 2 via
[Anaconda Cloud](https://anaconda.org/mw/pymol):
```bash
$ conda install -c schrodinger pymol
```

### macOS

In macOS, PyMOL requires XQuartz installation.

Install Xquartz via
[_Homebrew_](https://formulae.brew.sh/cask/xquartz).
```bash 
$ brew cask install xquartz
```
Or, 

An XQuartz installer is provided [here](https://www.xquartz.org/).

You must install PyMOL through [Anaconda](https://www.anaconda.com/)
package management system.

Follow these steps:

**1.** Create a new environment and activate it: 
```bash
$ conda create --name pymol2 python=3.7
$ conda activate pymol2
```

**2.** Install PyMOL v2 via
[Anaconda Cloud](https://anaconda.org/mw/pymol):
```bash
$ conda install -c schrodinger pymol
```

**3.** Try PyMOL v2:
```bash
$ pymol
```

If the options above does not work, you can try an installation via
_Homebrew_ package manager. A brief description of how to install open
source PyMOL for macOS is provided [here](https://pymolwiki.org/index.php/MAC_Install).

---

## Code Description

Here, we provide a brief explanation on the `Qt` threads, classes and common HTTP responses of PyMOL KVFinder-web Tools.

The PyMOL KVFinder-web Tools source code organizes as follows:

```bash
PyMOL-KVFinder-web-Tools/
    LICENSE
    PyMOL-KVFinder-web-tools.ui
    README.md
    __init__.py
examples/
    1FMO.pdb
    1HHP.pdb
    1HVR.pdb
    4P24.pdb
    ligs_1FMO.pdb
requirements.txt
```

The `__init__.py` file contains the signals, slots, functions, callbacks and classes and the `PyMOL-KVFinder-web-tools.ui` file contains the graphical user interface designed with `Qt Designer` of PyMOL KVFinder-web Tools. Further, the `requirements.txt` file contains the python modules and versions necessary to run PyMOL KVFinder-web Tools. Finally, there are some structures for testing in `examples` directory (_Note_: currently, `4P24.pdb` exceeds maximum payload of 1 Mb of KVFinder-web server).

### Threads

The PyMOL KVFinder-web Tools are composed of two `Qt` threads, including:

- **Graphical User Interface (GUI)** thread: the main `Qt` thread that handles user interactions with the visual interface;

- **Worker** thread: the worker `Qt` thread that constantly checks jobs sent to KVFinder-web server (currently local server at https://localhost:8081/) and automatically downloads completed jobs from server.

### Classes 

The PyMOL KVFinder-web Tools are composed of five classes:

1. `class PyMOLKVFinderWebTools(QMainWindow)`: loads `PyMOL-KVFinder-web-tools.ui`, bind callbacks to `QPushButton` and other `QtWidgets`, creates the communication between GUI and Worker threads by connecting `pyqtSlot` and `pyqtSignal`, create POST (https://localhost:8081/create) request to send jobs to KVFinder-web server, and define functions and callbacks;

2. `class Worker(QThread)`: create GET (https://localhost:8081/id) requests to KVFinder-web server with job IDs availables to retrieve results from KVFinder-web server and process these GET requests responses.

3. `class Form(QDialog)`: create a custom `QDialog` to create a form activated by clicking on 'Add ID' `QPushButton` and method to retrieve filled information on this form.

4. `class Message(QDialog)`: create a custom `QDialog` to pop up when a POST request is made to KVFinder-web server, showing a message, job ID, and job status (optional). 

5. `class Job(object)`: create the KVFinder-web job to be sent to KVFinder-web server. The class uploads(`upload(parameters)`) parameters from GUI in it, save(`.save(id))` and load(`.load(fn)`) `job.toml` file with information about the job for `Worker` thread operation, and export (`.export()`) files retrieved from GET response of a 'completed' job, including KVFinder results file (*.KVFinder.results.toml*), cavity PDB file (*.KVFinder.output.pdb*), log file (*KVFinder.log*) and parameters file (*parameters.toml* - optional).

### Common HTTP Responses

Responses (`QNetwork.QNetworkReply.error()`) from KVFinder-web server when `QtNetwork.AccessManager()` sents a `.get()` or `.post()` request:

- `QNetwork.QNetworkReply.NoError` (Response 0): Sucessfull `.get()` or `.post()` request;

- `QNetwork.QNetworkReply.ConnectionRefusedError` (Response 1): KVFinder-web server is currently offline or unreachable (e.g. no internet connection);

- `QNetwork.QNetworkReply.ContentNotFoundError` (Response 203): The remote content was not found at KVFinder-web server. Hence, the requested (`.get()` request) job ID does not exist or already been deleted on KVFinder-web server;

- `QNetwork.QNetworkReply.UnknownContentError` (Response 299): The `.post()` request entity is larger than limits defined by KVFinder-web server (currently, **1 Mb**).
