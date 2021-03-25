
set(TargetName CSharpDLL)

file(GLOB_RECURSE SOURCES *.cs) 

# Define the DLL target, including all relevant project files.
add_library(${TargetName} SHARED ${SOURCES})

# Set the .NET Framework version for the target.
set_property(TARGET ${TargetName} PROPERTY VS_DOTNET_TARGET_FRAMEWORK_VERSION "v4.6.1")
# Set the C# language version (defaults to 3.0 if not set).
set(CMAKE_CSharp_FLAGS "/langversion:6")
# Add in some .NET reference libraries.
set_property(TARGET ${TargetName} PROPERTY VS_DOTNET_REFERENCES
    "Microsoft.CSharp"
    "System"
    "System.Core"
    "System.Data"
    "System.Deployment"
    "System.Drawing"
    "System.Net.Http"
    "System.Xml"
)