{
  "targets": [
      {
          "target_name": "evalExpr",
          "sources": [
              "src/addon/eval-expr.cc",
          ],
          "include_dirs": [
            "<!@(node -p \"require('node-addon-api').include\")"
          ],
          'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
      }
  ]
}