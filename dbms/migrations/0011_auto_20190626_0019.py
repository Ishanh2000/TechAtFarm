# Generated by Django 2.2.2 on 2019-06-26 00:19

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('dbms', '0010_auto_20190625_1749'),
    ]

    operations = [
        migrations.AlterField(
            model_name='request',
            name='accepted',
            field=models.CharField(choices=[('T', 'True'), ('F', 'False'), ('p', 'Pendg')], max_length=1),
        ),
    ]