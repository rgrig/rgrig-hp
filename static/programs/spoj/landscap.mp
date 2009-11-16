n=2.5mm; % node size for tree
g=4*n; % grid size for tree
u=3mm; % grid size for landscape

def node (expr x, y) (text t) =
  draw (x*g-n,y*g)..(x*g,y*g-n)..(x*g+n,y*g)..(x*g,y*g+n)..cycle;
  label(t, (x*g,y*g));
enddef;

def edge (expr x, y, xx, yy) =
  dx:=(xx-x)*g; dy:=(yy-y)*g; d:=dx++dy;
  ddx:=dx/d*n; ddy:=dy/d*n;
  draw (x*g+ddx,y*g+ddy)--(xx*g-ddx,yy*g-ddy);
enddef;

def nodep (expr x, y, t, xp, yp) =
  node(x,y,t);
  edge(x,y,xp,yp);
enddef;

beginfig(0);
  node(0,0,"12");
  nodep(-1,1,"5",0,0);
  nodep(0,1,"3",0,0);
  nodep(1,1,"1",0,0);
  nodep(-1,2,"3",-1,1);
  nodep(0,2,"1",0,1);
endfig;

beginfig(1);
  string land[];
  land[0]:="  ***  *";
  land[1]:=" ***** *** *";
  land[2]:="************";

  draw (0,u)--(12u,u);
  draw (6u,u)--(6u,3u); draw (10u,u)--(10u,3u); draw (12u,u)--(12u,3u);
  draw (0,2u)--(10u,2u);
  
  pickup pencircle scaled 2mm;
  for i=0 upto 2:
    for j=0 upto 11:
      if (substring (j,j+1) of land[i])="*":
        draw (u/2 + j*u, u/2 + (2-i)*u);
      fi
    endfor;
  endfor
  
endfig;

end;
