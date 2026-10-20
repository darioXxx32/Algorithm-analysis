from pathlib import Path
p=Path('report_draft.tex'); s=p.read_text(encoding='utf-8')
s=s.replace(chr(92)*2+'texttt',chr(92)+'texttt').replace(chr(92)*2+'_',chr(92)+'_')
s=s.replace(chr(0x201c),'``').replace(chr(0x201d),"''")
p.write_text(s,encoding='utf-8')
