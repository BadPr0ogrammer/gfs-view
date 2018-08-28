# GFS-view-OS

The program to generate and display weather forecast maps from the NCEP data

## License

The *GFS-view-OS* is licensed under the **GPLv3** License

See *met-5.2_bugfix/README* for the **UCAR** License

The *NOAA NCEP Library for GRIB2 decoder* source code is provided **AS IS** for the use of NCEP data users

*Frank Warmerdam* Shapelib is licensed under the **LGPL** License

## Usage

+ Get the *gfs-view_os-master* repository, **unzip** into a *working dir*
+ Copy the whole **./GFS-view-OS** data directory (with *GRIB2* data, *Shape* files, *ini* and *log* files) from the working dir into:
	- On Windows: the *Documents folder* - **C:\Users\some_user_name\Documents**
	- On Linux: the *Home directory* - **/home/some_user_name**
+ On Linux: 
	* Install **Qt5.9 SDK with QtCreator** by a package manager or from the [Qt Company](https://www.qt.io/download-thank-you)
	* Install **OpenCV 2.4** library
	* Install **Shapelib** library
	* Install **libjasper,libjpeg,libpng**
	* Launch *qtcreator*, open the **gfs-view_os.pro** project and *build*
+ On Windows:
	- Use *gfs-view.exe* from the working dir
		* Install [Visual C++ Redistributable for Visual Studio 2017 (x64)](https://aka.ms/vs/15/release/vc_redist.x64.exe)
		* Go to the *./Release/bin* directory and start prebuild *gfs-view.exe*
	- Build *from the source*:
		- Install **Qt5.9 SDK** for the *msvc2017_64* architecture
		- Install **Visual Studio 2017**
		- Install **Qt Visual Studio Tools** from *Tools/Extensions and Updates*
		- Install **OpenCV** into the *..\opencv-2.4.13.6* directory,
			or correct *Include* and *Lib* directories for the *Project/Properties*
		- Open **gfs-view_os.sln** from the working dir and build *Release* configuration
		- Go to the *Release/bin* directory, start the *gfs-view.exe* you've made

## Contact

Send me your feedback, comments, and suggestions. I am looking forward to hearing from you!
email:[BadPr0ogrammer@gmail.com](BadPr0ogrammer@gmail.com)
