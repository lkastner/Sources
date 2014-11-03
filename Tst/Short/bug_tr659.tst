LIB "tst.lib";
tst_init();

// incomplete multivariate factorization over Q (tr.659)
// fixes also tr.655 (incomplete primary decomp with primdecGTZ),
// tr.656 (incomplete primary decomp with primdecSY)

ring R=0,(a,r,v,n,g,f,h,o),dp;
poly f=
       (248301045a2r10n2)*o10+(570807000a2r9n)*o9+(-137945025a2r8n2+328050000a2r8)*o8+(-253692000a2r7n)*o7+(30654450a2r6n2-109350000a2r6-22457088ar2vn2)*o6+(42282000a2r5n)*o5+(-3406050a2r4n2+12150000a2r4+2495232avn2)*o4+(-3132000a2r3n)*o3+(189225a2r2n2-450000a2r2)*o2+(87000a2rn)*o+(-4205a2n2);

factorize(f);

// tr.655 (incomplete primary decomp with primdecGTZ)
LIB"primdec.lib";
ring rng = (0),(a,r,v,n,g,t,h,o),dp;
ideal I = -30*a*v*t^3+2*t,
25*r*h+29*v*n^2*t*h^2+19,
-18*a*r^2*o^2+24*a*v*n*t*h*o+2*a;
list L1 = primdecGTZ (I);
L1[2][1];
list pc2 =  primdecGTZ(L1[2][1]);
pc2;
size(pc2); // must be 1
kill rng;

// tr.656 (incomplete primary decomp with primdecSY)
proc test656()
{
    ring rng656 = (0),(xy,xg,xt,xb),dp;
    ideal I = -11658/12181*xg*xt*xb-8219/674*xg*xb^2,
    307/3906*xy^2+20884/14685*xg^3+21229/26747*xg,
    -3493/11608*xy+13440/12053*xg*xt-1161/3359*xb-1377/857;
    def L1 =  primdecSY(I,0);
    def pc1 =  primdecSY(L1[1][1],0);
    ASSUME(0, size(pc1)==1);
}
int i ; 
// repeat test, since the computation originaly failed occasionally, independent of random seed...
for (i=1; i<12; i++)     {    test656();  }

tst_status(1);$
