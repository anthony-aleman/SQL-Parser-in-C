# SQL-Parser-in-C
I created LL(1) Parser using C11 for the purpose of querying simple csv files

The Following SQL input:
```<SQL>
SELECT ColumnName FROM File WHERE ColumnName = 'value';
```
Results in this output:

```
SELECT        
IDENTIFIER    
FROM
IDENTIFIER    
WHERE
IDENTIFIER    
EQUALS        
STRING_LITERAL
SEMICOLON
```
