from django.db import models
from django.core.signals import request_finished
from django.dispatch import receiver
from django.db.models.signals import post_save

class Data(models.Model):
    user_name = models.CharField(max_length=20, primary_key = True)
    name = models.CharField(max_length=20)
    mobile_number = models.CharField(max_length=10)
    residence = models.CharField(max_length=200)
    aadhar = models.CharField(max_length=12)
    password = models.CharField(max_length=12)
    def __str__(self):
        return self.user_name

class Land(models.Model):
    user_name = models.ForeignKey(Data, on_delete = models.CASCADE)
    land_name = models.CharField(max_length= 20)
    land_address = models.CharField(max_length=100)
    district = models.CharField(max_length = 20, default = 'Kanpur')
    soil_type = models.CharField(max_length = 20)
    crop_grown = models.CharField(max_length = 20)
    moisture_requirement = models.PositiveIntegerField(default=0)
    threshold_moisture = models.PositiveIntegerField(default=0)
    expected_yield = models.FloatField(default = 0.0)
    expected_price = models.FloatField(default = 0.0)
    system_id = models.CharField(max_length = 10, default = 'None')
    system_ip = models.GenericIPAddressField(default="0.0.0.0")
    average_moisture = models.FloatField(default = 0.0)
    last_update = models.DateTimeField(auto_now=False, auto_now_add=False, default='2019-06-17 10:47:00.000000')
    request_status = models.CharField(max_length = 7, choices = (('True','True'), ('False','False'), ('Pending', 'Pending')), default = 'False')
    def __str__(self):
        return "Username : %s | Land Name: %s | Crop Grown: %s" % (self.user_name, self.land_name, self.crop_grown)

class Blog(models.Model):
    blogger_name = models.CharField(max_length=100)
    blogger_description = models.TextField(default='No Description Available')
    blogger_email = models.EmailField(default='Not Available')
    blog_heading = models.CharField(max_length=50)
    blog_description = models.TextField(default='No Description')
    blog_data = models.TextField(default = 'No Text')
    blog_link_heading = models.CharField(max_length=100, default='No Heading')
    blog_link = models.URLField(default = 'No URLs/Websites')
    def __str__(self):
        return self.blog_heading

class Crop(models.Model):
    crop_name = models.CharField(max_length=100)
    moisture_requirement = models.IntegerField(default=0)
    threshold_moisture = models.IntegerField(default=0)
    recommendation = models.TextField(default="")

    def __str__(self):
        return self.crop_name

class Message(models.Model):
    name = models.CharField(max_length=30)
    mobile_number = models.CharField(max_length=10)
    subject = models.CharField(max_length=30)
    message = models.TextField(max_length=200)
    def __str__(self):
        return self.subject


class Request(models.Model):
    user_name = models.ForeignKey(Data, on_delete = models.CASCADE)
    land_name = models.CharField(max_length= 20)
    land_address = models.CharField(max_length=100)
    district = models.CharField(max_length = 20)
    request_status = models.CharField(max_length = 7, choices = (('True','True'), ('False','False'), ('Pending', 'Pending')), default = 'Pending')
    system_id = models.CharField(max_length = 10, default = 'None')
    def __str__(self):
        return "Username : %s | Land Name: %s" % (self.user_name.user_name, self.land_name)

class Served_Request(models.Model):
    user_name = models.ForeignKey(Data, on_delete = models.CASCADE)
    land_name = models.CharField(max_length= 20)
    land_address = models.CharField(max_length=100)
    district = models.CharField(max_length = 20)
    system_id = models.CharField(max_length = 10, default = 'None')
    def __str__(self):
        return "Username : %s | Land Name: %s" % (self.user_name.user_name, self.land_name)

@receiver(post_save, sender = Request)
def pre_save_handler(sender, **kwargs):
    instance = kwargs.get('instance')
    land = Land.objects.get(user_name = instance.user_name, land_address = instance.land_address, land_name = instance.land_name, district = instance.district)
    land.request_status = instance.request_status
    land.system_id = instance.system_id
    land.save()
    if instance.request_status == 'True' and instance.system_id != 'None':
        req = Served_Request(
            user_name = instance.user_name, 
            land_address = instance.land_address, 
            land_name = instance.land_name, 
            district = instance.district,
            system_id = instance.system_id,
            )
        req.save()
        instance.delete()