cmake_minimum_required(VERSION 3.18)

project(dialog 
	VERSION 1.0.0
	DESCRIPTION "C++ library with class Dialog for work with class User."
	LANGUAGES CXX)

file(GLOB
	public_headers "*.h"
	sources "*.cpp")

add_library(${PROJECT_NAME}_lib ${sources} ${public_headers})
