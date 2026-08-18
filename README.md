# Fga721ExampleMod
this is a repo for lib pvz2 by inline hooking, using and64inlinehook as base, you can inject the library to the game and enjoy of features

## Features
this lib has features for modders, like:
* PlantNameMapper and ZombieAlmanac for add new integerids!
* Piano Softcode: You can put new zombies for dance with the piano!
* Healer and Magician Inmunities to shrink softcoded! (It didnt work with healer, so idk)

## How to inject?
arm64 is different to arm, so you can inject it by this steps:
* Decompile the apk via apktool or any other thing
* Delete the armeabi-v7a folder (or it will crash)
* In arm64-v8a, you must paste the libFga721.so after compiling it with visual studio
* In apk\smali_classes2\com\popcap\PvZ2\PvZ2GameActivity.smali, after load the pvz2lib (const-string v0, "PVZ2"), after the invoke_static you have to paste this:

      const-string v0, "Fga721"

      .line 57
      invoke-static {v0}, Ljava/lang/System;->loadLibrary(Ljava/lang/String;)V

  ## CAUTION:
  the owner of that project, isnt responsable for the improper use of that library, all the responsability for damages, hacking or improper monetization, is for the user who compiles and distributes it, because this library isnt damaging the integrity of the apk user, only is a test for hooking simple functions
