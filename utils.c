#include "utils.h"
#include <stdlib.h>
#include <stdio.h>

void check_heap(void* object){
    if(object == NULL){
        perror("Errore nell'allocazione della memoria.");
        exit(EXIT_FAILURE);
    }
}
/*
void print_logo(){
printf("                                                                                           \n         ");
printf("                                                  -y.    os    .y+    /y.    ss    .y+    /y.    so\n ");
printf("                                                  -ydo/+hMMdo/omMMy/+sNMmo++hMMd+/omMMy/+sNMmo/+hh+\n ");
printf("                                                    .NMMMNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNMMM+   \n");
printf("                                                    :NMMo                                    NMMs   \n");
printf("                                             -hNNmdmMMMM+            .sd-                    NMMMNs \n");
printf("                                            -NMMMMMMMMMM+          /hNMMN:                   NMMy-  \n");
printf("                                            yMMMMMMMMMMM+        /mMMMMMN.                   NMM/   \n");
printf("                                           :MMMNMMMMMMMM+      .yMMMMMMm-                    NMMNy/ \n");
printf("                                            /o-./dMMMMMMm.    -mMMMMMMN/                     NMMmo: \n");
printf("                                                  oMMMMMMy   -mMMMMMMd:                      NMM/   \n");
printf("                                                   yMMMMMMo .dMMMMMMs                        NMMd:. \n");
printf("   .-::-                                   .::    -hNMMMMMMddMMMMMm/                         NMMMdo \n");
printf("  /ds+oyd/                                 -ym      -NMMMMMMMMMMMy.                          NMMo   \n");
printf("  yN-   :::/:  ::  :/:  :/+/-   +/-//.  -/++ym      -NMMMMMMMMMN+                            NMMo   \n");
printf("  .oyyys/ -ms -mm- yd- hy:-:hy  +My:+. sd/-:ym    -hNMMMMMMMMMm-                             NMMMdo \n");
printf("  ::  -/Ny /m.h/oy-m: /M-   :M- -M:    No   om     -oMMMMMMMMMo                              NMMh:. \n");
printf("  sm+-:+mo  hdh  dds  .dy--:hh  /M+.   yd:.-ym:      mMMMMMMMMN:                             NMM/   \n");
printf("   -+oo+-   .+-  :+     :+o+:  .o++:    :+++/++   .+hMMMMMMMMMMN/                            NMMNs: \n");
printf("                                                  +MMMMMMmhMMMMMN/                           NMMNy/ \n");
printf("                                                :hMMMMMMy. yMMMMMN+                          NMM/   \n");
printf("                                              -yMMMMMMMM+   dMMMMMMs                         NMMy-  \n");
printf("                                            -sNMMMMMMMMM+   .dMMMMMMy                        NMMMmo \n");
printf("                                           /MMMMMMNmoNMM+    .mMMMMMMd.                      NMMs.  \n");
printf("                                           sMMMMMd- .NMM+     -NMMMmmdo                      NMM+   \n");
printf("                                           omNNh+ -yNMMM+      :hdd/                         NMMMd+ \n");
printf("                                                   :sMMM+                                    NMMd/- \n");
printf("                                                     mMMdhyyhyyhyhhyhyyhyyhyyhyyhyhyyhyyhyyhhMMM:   \n");
printf("                                                   /ymhdNMMNdhmMMMNhhNMMMmhdNMMMdhmMMMNhhNMMMmhdd+- \n");
printf("                                                  /N+   .mN-   /Nh    yN+   .mN.   /Nh    yN+   .mh \n");
printf("\n");
}
*/