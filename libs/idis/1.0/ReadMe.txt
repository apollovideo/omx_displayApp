********************************
** IDIS RDK Sampler / Tester  **
********************************

<< Package File Hierarchy Description >>

[root] -+-[Bin]
        +-[Include]
        |
        +-[Lib]------+--[arm]
        |            |
        |            +--[win32]
        |
        +-[Sampler]--+--[R1]
        |            |
        |            +--[Foundation]
        |
        +-[Tester]---+--[SDL2]
                     |
                     +--[win32]


  0. [root      ] Contains this readme.txt file which describes essential facets to guide easy usage of 
                  packaged files.

  1. [Bin       ] Tester Builds will be created in this path.

  2. [Include   ] Contains SDK header files, as listed below.

                  IDIS_Define.h
                  IDIS_DecoderSDK.h
                  IDIS_WatchSDK.h
                  Win32Porting.h

                  1) IDIS_Define.h 
                    : This defines commonly used constants, enums, structs among IDIS SDKs.
                  2) IDIS_DecoderSDK.h
                    : The header file for libdecoder.so / idisDecoder.dll.
                      This defines Decoder SDK APIs, and its parameter structs.
                  3) IDIS_WatchSDK.h
                    : The header file for libwatch.so / idisWatch.dll 
                      This defines Watch SDK APIs, callbacks, and parameter types used by them.
                  4) Win32Porting.h
                    : This header is required for *nix target build. Defines equivalents for 
                      win32 types in *nix system, that are used for SDKs.

  3. [Lib       ] Contains built *.so files. Which is *nix equivalent for idis SDKs, such as idisWatch.dll,
                  or idisDecoder.dll.
  4. [Lib/arm   ] Contains ARM target SDK binaries build
  5. [Lib/win32 ] Contains Win32 target SDK binaries build

  6. [Sampler   ] Contains R1/Foundation C++ implemenetations for supporting easy use of idis SDKs. 
                  These files are part of RDK.

  7. [Sampler/Foundation] Contains below files listed. Most of files are to provide safe global object instancing,
                  primitive general types, and thread synchronization. 
                  * Note that boost library is required to build properly.

                  1) R2AutoGaurd.h      : Scoped lock implementation, using RAII.
                  2) R2ChannelSet.h/hpp : A class to handle channels, this can be often used for camera channels,
                                          however not limited to camera.
                  3) R2ConfigPlatform.h : Win32 defines for supported platform configuration.
                  4) R2General.h        : Reserved header for general use.
                  5) R2GeneralMacro.h   : Macro defines to ease the use of RDK.
                  6) R2GeneralType.h    : Defines to ease the use of primitive types when developing with RDK.
                  7) R2SharedLock.h/hpp : A shared mutex implementation.
                  8) R2Singleton.h/hpp  : A singleton pattern implementation.
                  9) R2SpinLock.h/hpp   : A spin lock implementation.


  8. [Sampler/R1] Contains below files listed. These files have C++ implementation to aid easy use of idis SDKs,
                  and provide dynamic loading of libraries and their visible exported APIs.
                  * Note that boost library is required to build properly.

                  1) R1Watch.h/cpp      : A Sampler implementation for libwatch.so / idisWatch.dll.
                                          This class acquires a single handle for SDK on construction, and 
                                          releases on destruction. This class dynamically loads library, and its 
                                          exported functions. 
                                          * This sampler class wraps only essential APIs, but not all exported functions.
                                            Handle returned from SDK can be used outside the class through public method 
                                            'getSafeHandle'.
                  2) R1WatchListener.h  : A listener interface class for receiving propagated callbacks from SDKs.
                  3) R1Decoder.h/cpp    : A Sampler implementation for libdecoder.so / idisdecoder.dll.
                                          This class acquires a single handle for SDK on construction, and 
                                          releases on destruction. This class dynamically loads library, and its 
                                          exported functions.
                                          * This sampler class wraps only essential APIs, but not all exported functions.
                                            Handle returned from SDK can be used outside the class through public method 
                                            'getSafeHandle'.
                  4) R1ConnectInfo.h    : A class to replace POD parameter struct in sampler class. This class instance is
                                          used as parameter in connection related wrapper methods.
                  5) R1HandleInstanceMap.h/hpp : This implementation is for managing sampler instance by SDK handle.

  9. [Tester    ] Cotains below files listed. These files are used to build simple tester app that can
                  be built in *nix target.

                  1) Watch.cpp           : A console tester app. This uses SDL2 library for displaying retrieved frames
                                           from SDK.
                  2) Makefile            : A Makefile to build *nix targeted tester app.
                  

 10. [Tester/SDL2 ] Contains SDL library *.so / *.a files and API headers. (omitted sub direcotry description for this item)

 11. [Tester/win32] Contains visual studio solution/project file for building and testing tester app.
                  
                  1) Watch.sln           : vc80 solution file (Visual Studio 2005)
                  2) Watch.vcproj        : vc80 porject file (Visual Studio 2005)

<< Raspberry Pi Tester Build >>
  0. Install Boost Library. ex) sudo apt-get install libboostX.xx-all-dev
  1. Copy Libraries to '/usr/lib'. (Lib/arm/*.so, Tester/SDL2/lib/arm/*.so)
  2. make.











