{
  "variables": {
    "cflags_cc": [
      "-Wall",
      "-Werror",
      "-O3",
      "-fexceptions",  # Boost on Linux wants this
      "-frtti"         # And this too.
    ],
    "include_dirs": [
      "/Users/wanqingying/Downloads/boost_1_83_0",
      "/usr/local/include",
      "/Users/wanqingying/IdeaProjects/node-addon/node-shared-map/node_modules/node-addon-api"
    ],
    "libraries": [], #
    "OTHER_CFLAGS": [  # for Mac builds
      "-Wno-unused-local-typedefs"
    ]
  },
  "targets": [
    {
      "target_name": "hello",
      "sources": [ "hello.cc" ],
      "cflags_cc": [ "<@(cflags_cc)" ],
      "include_dirs": [ "<@(include_dirs)" ],
      "libraries": [ "<@(libraries)" ],
        "xcode_settings": {
        "MACOSX_DEPLOYMENT_TARGET": "10.9",
        "OTHER_CFLAGS": [
          "<@(OTHER_CFLAGS)" ,
            "-stdlib=libc++"
        ],
        "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
        "GCC_ENABLE_CPP_RTTI": "-frtti"
      },
      "msvs_settings": {
        "VCLinkerTool": {
          "AdditionalLibraryDirectories": [
          ]
        }
      }
    }
  ],
    "conditions": [
      [
        "OS=='win'", {
          "variables": {
            "include_dirs": [
            ]
          }
        }
      ],
      [
        "OS=='linux'", {
          "variables": {
            "libraries": [
              "-lrt"
            ]
          }
        }
      ],
      
    ],
}
