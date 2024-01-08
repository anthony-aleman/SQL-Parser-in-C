# SQL-Parser-in-C
LL(1) Parser using C11

The Following SQL:
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
STRING_LITERALSEMICOLON
```
