{
  writeShellApplication,
  symlinkJoin,
  python3,
  git,
}:

let
  pythonEnv = python3.withPackages (
    ps: with ps; [
      requests
      unidiff
      gitpython
      termcolor # for gccrs-style
    ]
  );

  gccrs-mklog = writeShellApplication {
    name = "gccrs-mklog";
    runtimeInputs = [
      git
      pythonEnv
    ];
    text = builtins.readFile ../bash/gccrs-mklog.sh;
  };

  gccrs-commit-mklog = writeShellApplication {
    name = "gccrs-commit-mklog";
    runtimeInputs = [
      git
      pythonEnv
    ];
    text = builtins.readFile ../bash/gccrs-commit-mklog.sh;
  };

  gccrs-verify = writeShellApplication {
    name = "gccrs-verify";
    runtimeInputs = [
      git
      pythonEnv
    ];
    text = builtins.readFile ../bash/gccrs-verify.sh;
  };

  gccrs-fix-changelog = writeShellApplication {
    name = "gccrs-fix-changelog";
    runtimeInputs = [
      git
      pythonEnv
    ];
    text = builtins.readFile ../bash/gccrs-fix-changelog.sh;
  };

  gccrs-style = writeShellApplication {
    name = "gccrs-style";
    runtimeInputs = [
      git
      pythonEnv
    ];
    text = builtins.readFile ../bash/gccrs-style.sh;
  };

in
symlinkJoin {
  name = "gccrs-contrib-tools";
  paths = [
    gccrs-mklog
    gccrs-commit-mklog
    gccrs-verify
    gccrs-fix-changelog
    gccrs-style
  ];
}
