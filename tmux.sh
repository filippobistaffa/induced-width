for o in wmf mf miw md
  do for t in arbitrary matching paths regions scheduling #j30
    do ./loop.sh --dir log/$o -o $o adjacency/am-$t*.*
    done
  done
