using System.Data;
using Npgsql;
using Temperature.API.Repositories;
using Temperature.API.Services;

var builder = WebApplication.CreateBuilder(args);

// add services
var connStr = builder.Configuration.GetConnectionString("devdb");
builder.Services.AddTransient<IDbConnection>((provider) => new NpgsqlConnection(connStr));

builder.Services.AddTransient<ITemperatureRecordsRepository, TemperatureRecordsRepository>();
builder.Services.AddTransient<ITemperatureRecordsService, TemperatureRecordsService>();

builder.Services.AddControllers();
builder.Services.AddCors(options =>
{
    options.AddPolicy("AllowAll", policy =>
        policy.AllowAnyOrigin()
              .AllowAnyHeader()
              .AllowAnyMethod());
});

builder.Services.AddEndpointsApiExplorer();
builder.Services.AddSwaggerGen();

// add interceptors
var app = builder.Build();

if (app.Environment.IsDevelopment())
{
    app.UseSwagger();
    app.UseSwaggerUI();
}

app.UseRouting();
app.UseCors("AllowAll");
app.MapControllers();

app.Run();
