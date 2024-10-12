# UE5 HTTP Backend

This is a personal project made for learning.

Unreal engine plugin that implements **HTTP connections** with everything needed to send and receive data from a backend API server.
The plugin is developed in C++ but I ported my attention for the **ease of use on Blueprint**.

**This plugin is a work in progress and may be missing features or contains bugs**

### Features
- Actor Component used to connect to a backend
	- JWT Token authentification
	- Header customisation
	- GET/POST
- GameplayTask to send single request from anywhere
- Get json from struct (C++ and Blueprint)
- Create struct from json (C++ and Blueprint)

### TODO
- Implements PUT/DELETE/PATCH
- More authentifications systems ?
- Create routes from DataTable or DataAssets
