using Microsoft.AspNetCore.Mvc;
using Temperature.API.Models;
using Temperature.API.Services;

[ApiController]
[Route("api/[controller]")]
public class TemperatureController : ControllerBase
{
    private static readonly List<AddTemperatureRecordRequest> Data = [];

    private ITemperatureRecordsService _temperatureRecordsService;

    public TemperatureController(ITemperatureRecordsService temperatureRecordsService)
    {
        _temperatureRecordsService = temperatureRecordsService;
    }

    [HttpGet]
    public IActionResult Get()
    {
        Console.WriteLine("GET request");
        return Ok(Data);
    }

    [HttpPost]
    public IActionResult Post([FromBody] AddTemperatureRecordRequest request)
    {
        _temperatureRecordsService.Add(request);
        return Ok(new { status = "ok" });
    }
}