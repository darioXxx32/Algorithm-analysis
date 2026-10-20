import csv, math
from pathlib import Path
p=Path('results/csv')
def read(name):
 with (p/name).open(newline='') as f:return list(csv.DictReader(f))
summary=read('summary.csv')
fail=[]
for r in summary:
 if r['function']=='reddi': continue
 fn=r['function']; name={'Adam':'adam','SGD+Momentum':'sgdm','AMSGrad':'amsgrad'}[r['optimizer']]
 rows=read(f'{fn}_{name}.csv')
 final=rows[-1]
 if int(final['iteration'])!=int(r['final_iter']): fail.append((fn,name,'iteration',final['iteration'],r['final_iter']))
 if abs(float(final['objective'])-float(r['final_objective']))>2e-9: fail.append((fn,name,'objective',final['objective'],r['final_objective']))
 if abs(float(final['grad_norm'])-float(r['final_grad_norm']))>2e-9: fail.append((fn,name,'grad',final['grad_norm'],r['final_grad_norm']))
 if abs(float(final['dist_to_opt'])-float(r['final_dist']))>2e-9: fail.append((fn,name,'distance',final['dist_to_opt'],r['final_dist']))
 print(fn,name,'rows',len(rows),'final_iter',final['iteration'],'conv',r['converged'],'max_grad',max(float(q['grad_norm']) for q in rows if q['iteration']!=''))
print('main mismatches',fail)
for rate in read('lr_sweep_rosenbrock.csv'):
 print('lr',rate['learning_rate'],'iter',rate['final_iter'],'norm',rate['final_grad_norm'],'conv',rate['converged'])
# Independently reproduce Reddi updates from its exact prescribed gradient sequence.
for alg in ('Adam','AMSGrad'):
 beta1,beta2,lr,eps=.9,.99,.01,1e-8
 x=m=v=vmax=0.; t=0
 for i in range(1,300001):
  g=101. if i%101==1 else -1.
  t+=1; m=beta1*m+(1-beta1)*g; v=beta2*v+(1-beta2)*g*g
  if alg=='Adam': upd=lr*(m/(1-beta1**t))/(math.sqrt(v/(1-beta2**t))+eps)
  else:
   vmax=max(vmax,v); upd=lr*m/(math.sqrt(vmax)+eps)
  x=max(-1.,min(1.,x-upd))
 print('Reddi reference',alg,format(x,'.12f'))
 rows=read('reddi_'+alg.lower()+'.csv')
 print('CSV tail',rows[-2:])
