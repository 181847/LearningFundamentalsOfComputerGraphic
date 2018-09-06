-- this is initialization script for current application. this will be run at the time application startup.
print("prepare lua")
package.path = package.path..";../?.lua"
package.cpath = package.cpath..";../?.dll"

controllerName = "controller"
controllerFilePath = package.searchpath(controllerName, package.path)
print("controller has found "..controllerFilePath)

-- add you codes here


print('initialize script complete')
