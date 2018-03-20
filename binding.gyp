{
  "targets": [
    {
      "include_dirs": [
        "<!(node -e \"require('nan')\")"
      ],
      "target_name": "addon",
      "sources": [
        "src/Process.cpp",
        "src/GetCurrentProcessWorker.cpp",
        "src/SetToForegroundWorker.cpp",
        "src/GetProcessesByNameWorker.cpp",
        "src/GetProcessByIdWorker.cpp",
        "src/Helpers.cpp"
      ],
      "msbuild_settings": {
        "ClCompile": {
          "CompileAsManaged": "true",
          "ExceptionHandling": "Async"
        }
      }
    }
  ]
}