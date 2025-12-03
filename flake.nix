
{
  description = "A Nix-flake-based C/C++ development environment";

  inputs.nixpkgs.url = "https://flakehub.com/f/NixOS/nixpkgs/0"; # stable Nixpkgs

  outputs =
    { self, ... }@inputs:

    let

	  system = "x86_64-linux";
      pkgs = import inputs.nixpkgs { inherit system; };

    in
    {
      devShells.${system}.default = pkgs.mkShell{
        
		buildInputs = with pkgs; [
            clang-tools
            cmake
            codespell
            conan
            cppcheck
            doxygen
		    gcc
			glew
		    glfw
            gtest
            lcov
			libGL
	      	libGLU
		    mesa
			pkg-config
            vcpkg
            vcpkg-tool
			xorg.libX11
	      	xorg.libXrandr
	      	xorg.libXi
	      	xorg.libXcursor
	      	xorg.libXinerama
            ]
            ++ (if system == "aarch64-darwin" then [ ] else [ gdb ]);
        };
    };
}