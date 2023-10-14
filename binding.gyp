{
  "targets": [
    {
      "target_name": "hello",
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "sources": [ "hello.cc" ],
      "include_dirs": [
        "C:/Users/wanqingying/AppData/Roaming/npm/node_modules/node-addon-api"
      ],
      'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
    }
  ]
}
