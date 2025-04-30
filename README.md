# Trabalho compiladores
Esse é um trabalho realizado pelo aluno Bruno Mamede de Araújo Moura para a matéria de compiladores ministrada pelo professor Flávio da Universidade Federal de São João Del Rei.

Antes de prosseguir, é importante ressaltar que todo o projeto está sendo desenvolvido em ambiente Linux, mais especificamente no sistema operacional Big Linux. Assim, a compilação para quaisquer outras plataformas não é garantida funcionar.

# Como compilar
Para compilar o trabalho, recomenda-se que crie uma pasta build para que não se misture os arquivos de compilação com os arquivos do projeto. Depois de decidida a pasta onde o programa vai ser compilado, execute os seguintes comandos em ordem:
```
cmake <caminho para o arquivo CMakeLists.txt>
make
```

Assim, um executável chamado "futz" terá sido criado e a partir daí você pode usar o compilador.

# Como executar
Primeiramente você deve chamar o executável "./futz" com o primeiro argumento sendo o caminho para o arquivo com o ponto de entrada *main*. Caso você passe um caminho de um arquivo .futz sem ponto de entrada, ele apenas testa a compilação e indica os erros (esse é o comportamento previsto, como o compilador ainda não está pronto, ter ou não a main no arquivo ainda não faz diferença). Após isso você pode inserir algumas flags, como por exemplo:

*--include* ou *-i*: usada para adicionar diretórios onde o compilador pode buscar por arquivos .futz no momento de importar novo código. Depois da flag, espera-se que diretórios sejam postos em sequência (parcialmente implementado: ele testa a validade dos diretórios, mas ainda não faz nada além disso).

*--output_tokens* ou *-t*: usada para criar um arquivo que conterá todos os tokens gerados pelo compilador, útil para debug. Logo após essa flag, você pode passar um diretório, no qual será criado um arquivo chamado tokens.txt ou pode passar um arquivo com a extensão .txt para escrever os tokens nele.

# documentação da linguagem
A linguagem por si só está sendo feita aos poucos. Por mais que já exista um documento anotando todos os seus recursos, tudo que até então foi planejado ainda está sujeito a mudanças, portanto, nada detalhado sobre ela será incluso no repositório (mas ainda será apresentado em sala de aula).
