# Generated by Django 2.2.9 on 2020-02-06 14:25

import datetime
from django.db import migrations, models
from django.utils.timezone import utc


class Migration(migrations.Migration):

    dependencies = [
        ('users', '0007_auto_20200205_2021'),
    ]

    operations = [
        migrations.AlterField(
            model_name='ride',
            name='arrival_time',
            field=models.DateTimeField(default=datetime.datetime(2020, 2, 6, 14, 25, 32, 523342, tzinfo=utc)),
        ),
    ]
