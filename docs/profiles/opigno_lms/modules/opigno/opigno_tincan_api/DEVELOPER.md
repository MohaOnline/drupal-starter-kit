# Creating a new statement
## The model
First, think about the construction of the statement. It will be easier after that to do the programming part.

Use this model for a start

```PHP
/**
 * - When user finish a quiz
 * Actor: user
 * Verb: xAPI/passed || xAPI/failed
 * Object: xAPI/lesson
 * Result: Get final quiz result
 * Context:
 *   Parent: Course
 */
```

## The programming part
After creating the model, use the functions provided by the file *includes/opigno_tincan_api.statements_func.inc*.

Most of the time, the statements sending will be in three parts:
1. The Statement creation. You can use `_opigno_tincan_api_statement_base_creation()` to create a basic statement with
  an actor, a verb and a node as object (node title and node ID will be used).
2. Adding everything more, like a context, a result or anything else as described in [this document](https://github.com/adlnet/xAPI-Spec/blob/master/xAPI.md).
3. Sending statement. Use the function `_opigno_tincan_api_send_statement()` to do that easily.
