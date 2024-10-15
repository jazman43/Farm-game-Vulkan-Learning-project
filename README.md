# Farm-game-Vulkan-Learning-project
teaching my self the vulkan API :)

---
## Issues

---
## Fixes

- imagesInFlight var was not being intialised and would cause a segmentation fault in the SubmitCommandBuffer fuction by resizing it to our swapChainImages size we have fixed this issue.
- the vkResetFences function was not added .
- vetex buffer and vertex memory buffers were not being deleted.

---
## New Features

