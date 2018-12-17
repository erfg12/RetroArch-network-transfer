This is an upload/download files system for RetroArch. This uses the default encryption system, and uses PHP for receiving/send files from the server.

If you would like to use this for a save file sync system, here's where you can place the functions:

- cloud_download should happen in the task_content.c file in the task_load_content function
- cloud_upload should happen in the task_save.c file in the task_save_handler_finished function

Special Thanks to hollow87 for helping.