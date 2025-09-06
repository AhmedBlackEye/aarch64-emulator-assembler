
#!/bin/bash

(
  cd ./emulate
  cb --clean
)

(
  cd ./assemble
  cb --clean
)

(
  cd ./common
  cb --clean
)