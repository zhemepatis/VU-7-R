using Temperature.API.Models;

namespace Temperature.API.Repositories;

public interface ITemperatureRecordsRepository
{
    public void Add(AddTemperatureRecordDto dto);
}