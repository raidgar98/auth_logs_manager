# auth_logs_manager
Very simple manager for auth logs. If u need to record time you spent in work / school and you are not working in midnight it should work. After data is collected you can export it to 'xlsx' and you have ready to report you work time. I recommend to archive and delete logs every month.

After you clone, go with second option here:
https://github.com/dbzhang800/QtXlsxWriter

In `.pro` file include is added, so just make sure that proper directory structure is provided.

If compiler report error about QVector -> QList unavaiable convertion, just replace QList with QVector in pointed line.

Thank you very much dbzhang800 for this masterpiece: QtXlsxWriter
