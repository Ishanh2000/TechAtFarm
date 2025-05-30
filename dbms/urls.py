from django.urls import path

from . import views

urlpatterns = [
    path('', views.index, name='index'),
    path('signup/', views.signup, name='signup'),
    path('login/', views.login, name='login'),
    path('submit/', views.submit, name = 'submit'),
    path('dashboard/', views.dashboard, name='dashboard'),
    path('dashboard/newLocation/', views.add_land, name='add_land'),
    path('dashboard/logout/', views.logout, name = 'logout'),
    path('dashboard/request_irrigation/', views.request_irrigation, name = 'request_irrigation'),
    path('dashboard/cancel_request/', views.cancel_request, name = 'cancel_request'),
    path('dashboard/deactivate_request/', views.deactivate_request, name = 'deactivate_request'),
    path('dashboard/delete_land/', views.delete_land, name = 'delete_land'),
    path('farmerSearch/', views.farmerSearch, name = 'farmerSearch'),
    path('message/', views.message, name = 'message'),
    path('device/', views.device, name = 'device'),
    path('dashboard/edit/', views.edit_land, name = 'edit_land'),
]
