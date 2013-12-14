# Raise window

Raise window by program name:

    run_or_raise gvim

...will find the first open gvim from the current XWindow session and bring it to front. 
If gvim is not started it will start another gvim process.

## Why?

For example, if you use keybindings to start an application and don't want to start multiple instances when you press the button twice, set the binding to start:

    run_or_raise your_target_app

...instead of just your\_target\_app.
