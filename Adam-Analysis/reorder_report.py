from pathlib import Path
p=Path('docs/report_draft.tex')
s=p.read_text(encoding='utf-8-sig')
s=s.replace('\\end{document}', '')
# Extract the current bibliography and submission block, then move them to the document end.
beg='\\begin{thebibliography}{3}'
end='\\end{thebibliography}'
bi=s.index(beg); ei=s.index(end,bi)+len(end)
bib=s[bi:ei]
s=s[:bi]+s[ei:]
link='\\section*{Submission Links}'
li=s.index(link)
# remove the link section through the end of its three lines
le=s.index('Recorded presentation:',li)
le=s.index('\n',le)
links=s[li:le]
s=s[:li]+s[le:]
# append verified expansion at end of the body
exp=Path('docs/verified_expansion.tmp').read_text(encoding='utf-8-sig')
s=s.rstrip()+'\n\n'+exp.strip()+'\n\n'+links+'\n\n'+bib+'\n\\end{document}\n'
# Remove the earlier, premature conclusion summary section.
start=s.find('\\section{Conclusions and Limitations}')
if start >= 0:
    nxt=s.find('\\section{Research Questions and Hypotheses}',start)
    if nxt >= 0: s=s[:start]+s[nxt:]
p.write_text(s,encoding='utf-8')
