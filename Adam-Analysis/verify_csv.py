import csv, math
from pathlib import Path
root=Path('results/csv')
for stem,fn,opt in [
 ('quadratic_2d_adam','quadratic',(0.,0.)),('quadratic_2d_sgdm','quadratic',(0.,0.)),('quadratic_2d_amsgrad','quadratic',(0.,0.)),
 ('rosenbrock_adam','rosenbrock',(1.,1.)),('rosenbrock_sgdm','rosenbrock',(1.,1.)),('rosenbrock_amsgrad','rosenbrock',(1.,1.)),
 ('rastrigin_adam','rastrigin',(0.,0.)),('rastrigin_sgdm','rastrigin',(0.,0.)),('rastrigin_amsgrad','rastrigin',(0.,0.))]:
 p=root/(stem+'.csv')
 with p.open(newline='') as f: rows=list(csv.DictReader(f))
 r=rows[-1]; x=float(r.get('x0',r.get('x',0))); y=float(r.get('x1',r.get('y',0)))
 if fn=='quadratic': val=x*x+y*y; grad=math.hypot(2*x,2*y)
 elif fn=='rosenbrock': val=(1-x)**2+100*(y-x*x)**2; grad=math.hypot(-2*(1-x)-400*x*(y-x*x),200*(y-x*x))
 else: val=20+(x*x-10*math.cos(2*math.pi*x))+(y*y-10*math.cos(2*math.pi*y)); grad=math.hypot(2*x+20*math.pi*math.sin(2*math.pi*x),2*y+20*math.pi*math.sin(2*math.pi*y))
 print(p.name, 'last_iter',r['iteration'],'rows',len(rows),'loss_error',abs(val-float(r['objective'])),'grad_error',abs(grad-float(r['grad_norm'])),'dist_error',abs(math.hypot(x-opt[0],y-opt[1])-float(r['dist_to_opt'])))
for p in [root/'reddi_adam.csv',root/'reddi_amsgrad.csv']:
 with p.open(newline='') as f: rows=list(csv.DictReader(f))
 print(p.name, 'tail', rows[-2:])
