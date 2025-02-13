sealed class KimeraMode {
  const KimeraMode();
}

class Piano extends KimeraMode {
  int transpose = 0;
  Piano({this.transpose = 0});
}

class Flute extends KimeraMode {
  const Flute();
}

class Violin extends KimeraMode {
  const Violin();
}

class Trumpet extends KimeraMode {
  const Trumpet();
}
