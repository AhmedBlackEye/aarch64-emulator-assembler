#!/bin/bash

# Step 1: Clean using Code::Blocks project cleaner (if cb is Code::Blocks)
cb clean

# Step 2: Compile and link
cb emulate

# Step 3: Copy executable to test suite directory (with overwrite)
cp -f emulate /home/nilux/university/group_project/armv8_testsuite/solution/emulate

