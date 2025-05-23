{
  description = "wutype";
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
  };
  outputs =
    {
      self,
      nixpkgs,
    }:
    let
      supportedSystems = [
        "x86_64-linux"
        "aarch64-linux"
        "x86_64-darwin"
        "aarch64-darwin"
      ];
      forEachSupportedSystem =
        f:
        nixpkgs.lib.genAttrs supportedSystems (
          system:
          f {
            pkgs = import nixpkgs {
              inherit system;
            };
          }
        );
    in
    {
      packages = forEachSupportedSystem ({pkgs}:
      {
          default = pkgs.stdenv.mkDerivation {
            pname = "wutype";
            version = "1.0.0";
            src = ./.;
            nativeBuildInputs = with pkgs; [
              gcc
              gnumake
            ];
            buildInputs = with pkgs; [
              ncurses
            ];
            preBuild = ''
              mkdir -p build
            '';
            buildPhase = ''
              runHook preBuild
              make all
              runHook postBuild
            '';
            installPhase = ''
              runHook preInstall
              mkdir -p $out/bin
              cp build/wutype $out/bin/
              runHook postInstall
            '';
            postInstall = ''
              make clean || true
            '';
          };
        });
      devShells = forEachSupportedSystem (
        { pkgs }:
        {
          default = pkgs.mkShell {
            buildInputs = with pkgs; [
              ncurses
            ];
            packages = with pkgs; [
              gcc
              gnumake
            ];
          };
        }
      );
    };
}
