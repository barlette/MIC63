# MIC63
Trabalho para a disciplina de MIC63

O objetivo final seria um posicionador e roteador automático de células baseadas em transistores 7nm FinFET.

Os arquivos spices que descrevem as células são parseados e alocados em um grafo.
As entidades possíves são:
  Gate
  Fonte
  Dreno
  

Foi utilizado caminho de Euler para calcular o menor caminho sem quebra de difusão, tornando o grafo em uma Àrvore.
O roteamento é realizado utilizando o algoritmo A*.

São geradas imagens dos layouts.
