from django.contrib import admin
from .models import Data, Land, Blog, Crop, Request, Served_Request, Message

admin.site.register(Data)
admin.site.register(Land)
admin.site.register(Blog)
admin.site.register(Crop)
admin.site.register(Request)
admin.site.register(Served_Request)
admin.site.register(Message)
# Register your models here.
