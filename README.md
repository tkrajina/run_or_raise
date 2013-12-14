# Raise window

Raise window by program name:

    raise_window gvim

...will find the first open gvim from the current XWindow session and bring it to front.

For example, if you need to run xfce4-appfinder *if* it is not already open, use:

    raise_window xfce4-appfinder || xfce4-appfinder
