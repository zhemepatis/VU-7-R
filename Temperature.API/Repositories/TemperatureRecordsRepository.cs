using System.Data;
using Dapper;
using Temperature.API.Models;

namespace Temperature.API.Repositories;

public class TemperatureRecordsRepository : ITemperatureRecordsRepository
{
    private IDbConnection _connection;

    public TemperatureRecordsRepository(IDbConnection connection)
    {
        _connection = connection;
    }

    public void Add(AddTemperatureRecordDto dto)
    {
        _connection.Open();

        var statement = 
        """
            insert into temperature_records (temperature)
            values (@Temperature)
        """;

        _connection.Execute(statement, dto);
    }
}