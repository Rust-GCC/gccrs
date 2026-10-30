{
  writeShellApplication,
  symlinkJoin,
  python3,
  git,
  useShortPrefix,
}:

let
  prefix = if useShortPrefix then "g" else "gccrs-";
  
  pythonEnv = python3.withPackages (
    ps: with ps; [
      requests
      unidiff
      gitpython
      termcolor # for gccrs-style
    ]
  );

  gccrs-mklog = writeShellApplication {
    name = "${prefix}mklog";
    runtimeInputs = [
      git
      pythonEnv
    ];
    text = builtins.readFile ../bash/gccrs-mklog.sh;
  };

  gccrs-commit-mklog = writeShellApplication {
    name = "${prefix}commit-mklog";
    runtimeInputs = [
      git
      pythonEnv
    ];
    text = builtins.readFile ../bash/gccrs-commit-mklog.sh;
  };

  gccrs-verify = writeShellApplication {
    name = "${prefix}verify";
    runtimeInputs = [
      git
      pythonEnv
    ];
    text = builtins.readFile ../bash/gccrs-verify.sh;
  };

  gccrs-fix-changelog = writeShellApplication {
    name = "${prefix}fix-changelog";
    runtimeInputs = [
      git
      pythonEnv
    ];
    text = builtins.readFile ../bash/gccrs-fix-changelog.sh;
  };

  gccrs-style = writeShellApplication {
    name = "${prefix}style";
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
