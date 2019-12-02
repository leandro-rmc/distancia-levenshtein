<h1>Distância Levenshtein</h1>
O algoritmo da distância de Levenshtein calcula quantas operações em uma palavra são necessárias para transformá-la em outra palavra. Exemplo: Para tranformar a palavra "casa" em "carro" são necessárias 2 substituições ('s' em 'r', 'a' em 'r') e a adição da letra 'o'. Portanto, podemos dizer que o custo total foi de 3 alterações (2 substituições e 1 adição). Muitos utilizam também um custo diferente para operações de substituição (2), pois uma alteração é como uma operação de deletar e inserir, mas aqui utilizarei o mesmo custo (1) para as 3 operações. O custo da substituição pode ser alterado rapidamente pela constante "CUSTO_SUBSTITUICAO".

<br>Resumo do funcionamento:<br>
<ul>
	<li>Usuário digita a quantidade de palavras similares a serem retornadas.</li>
	<li>Usuário digita uma palavra sem espaço.</li>
	<li>Algoritmo busca no arquivo "palavras.txt" as palavras mais similares a escolhida (as com menor custo) e as exibe na tela.</li>
</ul>
