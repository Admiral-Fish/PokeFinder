{
  description = "Cross platform Pokémon RNG tool";

  inputs = {
    nixpkgs.url = github:NixOS/nixpkgs/nixos-unstable;
  };

  outputs = { self, nixpkgs }: {
    packages.x86_64-linux.default =
      with import nixpkgs { system = "x86_64-linux"; };
      stdenv.mkDerivation {
        name = "PokeFinder";
        #version = "4.0.0";
        src = self;

        nativeBuildInputs = [
          cmake
          qt6.wrapQtAppsHook
        ];

        buildInputs = [
          python3
          qt6.qttools
          qt6.qtwayland
        ];

        cmakeFlags = [
          "\"-GUnix Makefiles\""
        ];

        preBuild = ''
          patchShebangs ../Source/Core/Resources/embed.py
        '';

        installPhase = ''
          mkdir -p $out/bin
          cp Source/Forms/PokeFinder $out/bin
        '';
      };
  };
}
